#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# Copyright (c) 2014, Yung-Yu Chen <yyc@solvcon.net>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of the pstake nor the names of its contributors may be
#   used to endorse or promote products derived from this software without
#   specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


"""Convert a pstricks LaTeX file (.tex) to an image file."""


import sys
import os
import warnings
import subprocess
import glob
import contextlib
import argparse
import tempfile
import shutil
import re
import collections

try:
    from docutils import nodes
    from docutils.parsers.rst import Directive
    from docutils.parsers.rst import directives, states
    from docutils.parsers.rst.roles import set_classes
    import sphinx
    HAS_SPHINX = True
except ImportError:
    HAS_SPHINX = False

try:
    from PIL import Image
    HAS_IMAGE = True
except ImportError:
    Image = None
    HAS_IMAGE = False

__version__ = "0.3.4+"


@contextlib.contextmanager
def _remember_cwd():
    curdir = os.getcwd()
    try:
        yield
    finally:
        os.chdir(curdir)


class ExternalCommand(object):
    def __init__(self, command, echo=True):
        self.command = command
        self.echo = echo

    @staticmethod
    def _which(program):
        """
        Taken from http://stackoverflow.com/a/377028
        """
        def is_exe(fpath):
            return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

        fpath, fname = os.path.split(program)
        if fpath:
            if is_exe(program):
                return os.path.abspath(program)
        else:
            for path in os.environ["PATH"].split(os.pathsep):
                path = path.strip('"')
                exe_file = os.path.join(path, program)
                if is_exe(exe_file):
                    return os.path.abspath(exe_file)

        return None

    @property
    def command_abspath(self):
        return self._which(self.command)

    def __call__(self, *args, **kw):
        cmdout = kw.pop('cmdout', None) # None is subprocess default.
        cmderr = kw.pop('cmderr', None) # None is subprocess default.
        param = " ".join(args)
        cmd = " ".join([self.command, param])
        if self.echo:
            sys.stdout.write(cmd + '\n')
            sys.stdout.flush()
        if cmdout and cmdout != sys.stdout:
            cmdout.write(cmd + '\n')
            cmdout.flush()
        with _remember_cwd():
            subprocess.call(cmd.split(), stdout=cmdout, stderr=cmderr)
            if cmdout:
                cmdout.flush()


_TEX_TEMPLATE = r'''\documentclass[%(options)sletterpaper,dvips]{article}
\usepackage[usenames]{color}
\usepackage{pst-all}
\usepackage{pst-3dplot}
\usepackage{pst-eps}
\usepackage{pst-coil}
\usepackage{pst-bar}
\usepackage{multido}
\usepackage{fancyvrb}
%(packages)s
\begin{document}
\pagestyle{empty}
\begin{TeXtoEPS}
%(code)s
\end{TeXtoEPS}
\end{document}'''


class Pstricks(object):
    def __init__(self, **kw):
        self.quiet = kw.pop('quiet', False)
        echo = not self.quiet
        self.tex_template = _TEX_TEMPLATE
        self.cmd_latex = ExternalCommand("latex", echo=echo)
        self.cmd_dvips = ExternalCommand("dvips", echo=echo)
        self.cmd_convert = ExternalCommand("convert", echo=echo)
        self.cmd_gs = ExternalCommand("gs", echo=echo)

    def write_tex(self, src, dst, cmbright=False, options=None, packages=None):
        # Sanitize options.
        options = ','.join(options if options else [])
        if options:
            options += ','
        # Sanitize packages.
        packages = [] if None is packages else packages
        if cmbright:
            cmbright = "\\usepackage{cmbright}"
            if not packages or cmbright not in packages:
                packages.insert(0, cmbright)
        packages = '\n'.join(packages)
        # Prepare TeX data.
        data = dict(options=options, packages=packages, code=src)
        tex = self.tex_template % data
        # Write the TeX data to the destination file.
        with open(dst, 'w') as tmpf:
            tmpf.write(tex)

    def pst(self, srcmain, dst, cmdout=None):
        self.cmd_latex(srcmain+'.tex', cmdout=cmdout)
        self.cmd_dvips(srcmain+'.dvi', '-q -E -o %s' % dst, cmdout=cmdout)

    def imconvert(self, src, dst, dpi=300, cmdout=None):
        """
        Currently I only support PNG.
        """
        if self.cmd_convert.command_abspath:
            self.cmd_convert('-density %d -units PixelsPerInch %s %s' % (
                dpi, src, dst), cmdout=cmdout)
        elif HAS_IMAGE:
            im = Image.open(src)
            size = tuple(float(dpi)/72 * val for val in im.size)
            self.cmd_gs(
                '-dSAFER', '-dBATCH', '-dNOPAUSE', '-dEPSFitPage',
                '-sDEVICE=pngalpha', '-g%dx%d' % size,
                '-sOutputFile=%s'%dst, src)
        else:
            raise OSError('either "%s" or PIL/Pillow installed' %
                          self.cmd_convert.command)

    def __call__(self, fn, cmbright=None, keep_tmp=None, cmdout=None,
                 options=None, **kw):
        with _remember_cwd():
            os.chdir(fn.tempdir)
            if not self.quiet:
                sys.stdout.write("Working in %s ...\n" % os.getcwd())
            cmbright = "cmbright" == cmbright
            if fn.sourcedata:
                sourcedata = fn.sourcedata
            else:
                with open(fn.sourcepath) as fobj:
                    sourcedata = fobj.read()
            self.write_tex(sourcedata, fn.intertex,
                           cmbright=cmbright, options=options)
            self.pst(fn.destmain, fn.intereps, cmdout=cmdout)
            if 'eps' == fn.ftype:
                shutil.copyfile(fn.intereps, fn.destpath)
            elif 'png' == fn.ftype:
                self.imconvert(fn.intereps, fn.destpath, cmdout=cmdout)
            else:
                if not self.quiet:
                    sys.stdout.write(
                        "Destination file type %s isn't supported.\n" % dstext)
        tempdir = os.path.abspath(fn.tempdir)
        if not keep_tmp:
            if not self.quiet:
                sys.stdout.write(
                    "Removing the temporary working directory %s ... " % tempdir)
            shutil.rmtree(fn.tempdir)
            if not self.quiet:
                sys.stdout.write("Done\n")
        else:
            if not self.quiet:
                sys.stdout.write(
                    "Temporary files were kept in the directory %s\n" % tempdir)


_FileParts = collections.namedtuple(
    '_FileParts',
    ('sourcedir', 'sourcedirabs', 'sourcebase', 'sourcemain',
     'destdir', 'destdirabs', 'destmain', 'destext'))

class Filename(object):

    @staticmethod
    def _source_determine(source, dest, ftype):
        """
        Determine :py:class:`filename parts <_FileParts>` from *source*.  The
        existence of *source* is assumed.  This is a utility method used by
        :py:meth:`__init__`.
        """
        # Sanitize source file name.
        if not source.lower().endswith(".tex"):
            source += ".tex"
        # Compute source directory.
        sourcedir, sourcebase = os.path.split(source)
        sourcedirabs = os.path.abspath(sourcedir)
        # Compute source file main name.
        sourcemain = os.path.splitext(sourcebase)[0]
        # Compute destination directory.
        if dest:
            destdir, destbase = os.path.split(dest)
        else:
            destdir = destbase = ''
        if not destdir and os.path.isdir(destbase):
            destdir, destbase = destbase, ''
        destdirabs = os.path.abspath(destdir)
        # Compute destination file name.
        if destbase:
            destmain, destext = os.path.splitext(destbase)
            destext = destext.strip('.')
            destext = destext if destext else ftype
        elif ftype:
            destmain, destext = sourcemain, ftype
        else:
            raise ValueError("either dest or ftype needs to be specified")
        if not destext:
            raise ValueError("can't determine the destination file type")
        # Sanitize destination file extension name.
        destext = destext.lower()
        # Return.
        return _FileParts(
            sourcedir, sourcedirabs, sourcebase, sourcemain,
            destdir, destdirabs, destmain, destext)

    @staticmethod
    def _dest_determine(source, dest, ftype):
        """
        Determine :py:class:`filename parts <_FileParts>` from *dest*.  The
        *source* is string data.  This is a utility method used by
        :py:meth:`__init__`.
        """
        # Set source path information to None.
        sourcedir = sourcedirabs = sourcebase = sourcemain = None
        # Compute destination directory.
        if dest:
            destdir, destbase = os.path.split(dest)
        else:
            raise ValueError("dest must be specified")
        if not destdir and os.path.isdir(destbase):
            raise ValueError("dest must not be a directory")
        destdirabs = os.path.abspath(destdir)
        # Compute destination file name.
        destmain, destext = os.path.splitext(destbase)
        destext = destext.strip('.')
        destext = destext if destext else ftype
        if not destext:
            raise ValueError("can't determine the destination file type")
        # Sanitize destination file extension name.
        destext = destext.lower()
        # Return.
        return _FileParts(
            sourcedir, sourcedirabs, sourcebase, sourcemain,
            destdir, destdirabs, destmain, destext)

    def __init__(self, source, dest='', ftype='',
                 source_not_path=False, tempdir=None):
        """
        This class is responsible for determining the file names to be used in
        :py:class:`Pstricks`.

        One should specify either (i) the *dest* argument with an extension
        name or (ii) the *ftype* argument.

        >>> Filename(source='dir1/main.tex', dest='dir2/', ftype='png')
        Filename(source='dir1/main.tex', dest='dir2/main.png')
        >>> Filename(source='dir1/main.tex', dest='other.png')
        Filename(source='dir1/main.tex', dest='other.png')
        >>> Filename(source='dir1/main.tex', dest='', ftype='png')
        Filename(source='dir1/main.tex', dest='main.png')

        Construction should fail if the extension name of the destination file
        can't be determined:

        >>> Filename(source='dir1/main.tex', dest='other')
        Traceback (most recent call last):
            ...
        ValueError: can't determine the destination file type
        >>> Filename(source='dir1/main.tex')
        Traceback (most recent call last):
            ...
        ValueError: either dest or ftype needs to be specified

        If both *dest* and *ftype* are given, the extension name of the
        destination file is deduced from *dest* if possible.  If *dest* doesn't
        contain the extension name, use *ftype*.

        >>> Filename(source='dir1/main.tex', dest='dir2/other.png', ftype='eps')
        Filename(source='dir1/main.tex', dest='dir2/other.png')
        >>> Filename(source='dir1/main.tex', dest='dir2/other', ftype='eps')
        Filename(source='dir1/main.tex', dest='dir2/other.eps')

        If source is raw input:

        >>> fnobj = Filename(source='raw data', dest='dir2/other.png',
        ...                  source_not_path=True)
        >>> fnobj
        Filename(source=None, dest='dir2/other.png')
        >>> fnobj.sourcedata
        'raw data'
        """
        # See :py:attr:`tempdir`.
        self._tempdir = tempdir
        #: String of raw source latex data.
        self.sourcedata = None
        # Interpret input.
        if source_not_path: # Determine filename parts from destination.
            self.sourcedata = source
            file_parts = self._dest_determine(source, dest, ftype)
        else: # Detemine filename parts from source.
            file_parts = self._source_determine(source, dest, ftype)
        # Set file_parts to self.
        #: Relative directory of the source file.
        self.sourcedir = file_parts.sourcedir
        #: Absolute directory of the source file.
        self.sourcedirabs = file_parts.sourcedirabs
        #: Base name of the source file.
        self.sourcebase = file_parts.sourcebase
        #: Main part of the base name of the source file.
        self.sourcemain = file_parts.sourcemain
        #: Relative directory of the destination file.
        self.destdir = file_parts.destdir
        #: Absolute directory of the destination file.
        self.destdirabs = file_parts.destdirabs
        #: Main part of the base name of the destination file.
        self.destmain = file_parts.destmain
        #: Extension part of the base name of the destination file.  It is
        #: always lower-case.
        self.destext = file_parts.destext

    def __repr__(self):
        if self.sourcedata:
            source = None
        else:
            source = "'%s'" % os.path.join(self.sourcedir, self.source)
        return "%s(source=%s, dest='%s')" % (
            self.__class__.__name__, source,
            os.path.join(self.destdir, self.dest))

    @property
    def tempdir(self):
        """
        Return a temporary directory.  If it doesn't exist, create a new one.

        The constructor can specify an existing directory as the temporary
        directory.

        Note: caller is responsible for deleting the created temporary
        directory.
        """
        if None is self._tempdir:
            self._tempdir = tempfile.mkdtemp()
        else:
            if not os.path.exists(self._tempdir):
                os.makedirs(self._tempdir)
            elif not os.path.isdir(self._tempdir):
                self._tempdir = tempfile.mkdtemp()
        return self._tempdir

    @property
    def source(self):
        """
        The source file name.  It is the same as :py:attr:`sourcebase`.

        >>> # Source determining.
        >>> Filename(source='dir1/name.tex', dest='dir2/', ftype='png').source
        'name.tex'
        >>> # Destination determining.
        >>> Filename(source='raw data', dest='dir2/name.png',
        ...          source_not_path=True).source
        """
        return self.sourcebase

    @property
    def dest(self):
        """
        The destination file name.

        >>> Filename(source='dir1/name.tex', dest='dir2/', ftype='png').dest
        'name.png'
        >>> Filename(source='dir1/name.tex', dest='dir2/', ftype='eps').dest
        'name.eps'
        """
        return '.'.join([self.destmain, self.destext])

    @property
    def sourcepath(self):
        """
        The absolute path to the source file.
        """
        if self.sourcedirabs is None or self.source is None:
            return None
        else:
            return os.path.join(self.sourcedirabs, self.source)

    @property
    def destpath(self):
        """
        The absolute path to the destination file.
        """
        return os.path.join(self.destdirabs, self.dest)

    @property
    def sourcerelpath(self):
        """
        The relative path to the source file.
        """
        if self.sourcedir is None or self.source is None:
            return None
        else:
            return os.path.join(self.sourcedir, self.source)

    @property
    def destrelpath(self):
        """
        The relative path to the destination file.
        """
        return os.path.join(self.destdir, self.dest)

    @property
    def sourceext(self):
        """
        Extension part of the base name of the destination file.
        """
        return "tex"

    @property
    def ftype(self):
        """
        Type of the destination file.

        It should be the same as :py:attr:`destext`.

        >>> Filename(source='dir1/name.tex', dest='name.png').ftype
        'png'
        """
        return self.destext

    @property
    def intertex(self):
        """
        The intermediate TEX file name for :py:meth:`Pstricks.pst`.

        >>> Filename(source='dir1/name.tex', ftype='png').intertex
        'name.tex'
        >>> Filename(source='dir1/name.tex', dest='dir2/other.png').intertex
        'other.tex'
        """
        return '.'.join([self.destmain, 'tex'])

    @property
    def intereps(self):
        """
        The intermediate EPS file name for :py:meth:`Pstricks.pst`.

        >>> Filename(source='dir1/name.tex', ftype='png').intereps
        'name.eps'
        >>> Filename(source='dir1/name.tex', dest='dir2/other.png').intereps
        'other.eps'
        """
        return '.'.join([self.destmain, 'eps'])


# Begin script code.
def main():
    """
    Main function when run as a script.
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-V', '--version', action='version',
                        version='%%(prog)s %s' % __version__)
    parser.add_argument('src', type=str,
                        help='source file name')
    parser.add_argument('dst', type=str, nargs='?', default=None,
                        help='destination file name')
    parser.add_argument('--font', dest='font', action='store',
                        default=None, choices=[None, 'cmbright'],
                        help='font selection (default = %(default)s)')
    parser.add_argument('-g', dest='options', action='store',
                        default=False, type=str,
                        help='global LaTeX options')
    parser.add_argument('-k', dest='keep_tmp', action='store_true',
                        default=False,
                        help='keep temporary directory')
    parser.add_argument('-t', dest='dstext', type=str, action='store',
                        default='png',
                        help='destination file type '
                             '(default = %(default)s)')
    parser.add_argument('-q', '--quiet', dest='quiet', action='store_true',
                        default=False,
                        help='suppress terminal output')
    parser.add_argument('--cmdout', dest='cmdout', action='store',
                        type=str, default=None,
                        help='sub-command output file '
                             '(default = %(default)s)')
    parser.add_argument('--tempdir', dest='tempdir', action='store',
                        default=None,
                        help='DANGEROUS! Be sure to set -k, '
                        'otherwise the temporary directory you specified '
                        'will be deleted after the script ends.')
    args = parser.parse_args()

    if args.quiet:
        args.cmdout = '/dev/null'
    if args.cmdout:
        args.cmdout = open(args.cmdout, 'a+')
    runner = Pstricks(**vars(args))
    fn = Filename(source=args.src, dest=args.dst, ftype=args.dstext,
                  tempdir=args.tempdir)
    runner(fn, **vars(args))
    if args.cmdout:
        args.cmdout.close()

    return 0


if __name__ == '__main__':
    main()
# End script code.


# Start Sphinx code.
if HAS_SPHINX:
    class pstake(nodes.image):
        pass


    class PstakeDirective(Directive):

        align_h_values = ('left', 'center', 'right')
        align_v_values = ('top', 'middle', 'bottom')
        align_values = align_v_values + align_h_values

        has_content = True
        required_arguments = 0
        optional_arguments = 1

        def align(argument):
            # This is not callable as self.align.  We cannot make it a
            # staticmethod because we're saving an unbound method in
            # option_spec below.
            return directives.choice(argument, PstakeDirective.align_h_values)

        def figwidth_value(argument):
            if argument.lower() == 'image':
                return 'image'
            else:
                return directives.length_or_percentage_or_unitless(
                    argument, 'px')

        option_spec = {
            'alt': directives.unchanged,
            'height': directives.length_or_unitless,
            'width': directives.length_or_percentage_or_unitless,
            'scale': directives.percentage,
            'align': align,
            'name': directives.unchanged,
            'target': directives.unchanged_required,
            'class': directives.class_option,
            'figwidth': figwidth_value,
            'figclass': directives.class_option,
        }

        def _check_align(self):
            if 'align' not in self.options:
                return
            if isinstance(self.state, states.SubstitutionDef):
                # Check for align_v_values.
                if self.options['align'] not in self.align_v_values:
                    raise self.error(
                        'Error in "%s" directive: "%s" is not a valid value '
                        'for the "align" option within a substitution '
                        'definition.  Valid values for "align" are: "%s".'
                        % (self.name, self.options['align'],
                           '", "'.join(self.align_v_values)))
            elif self.options['align'] not in self.align_h_values:
                raise self.error(
                    'Error in "%s" directive: "%s" is not a valid value for '
                    'the "align" option.  Valid values for "align" are: "%s".'
                    % (self.name, self.options['align'],
                       '", "'.join(self.align_h_values)))

        def _make_pstake_node(self):
            env = self.state.document.settings.env
            # Make pstake node.
            set_classes(self.options)
            pstake_node = pstake(self.block_text, **self.options)
            targetid = "pstake-%d" % env.new_serialno("pstake")
            pstake_node['targetid'] = targetid
            if len(self.arguments):
                pstake_node['texpath'] = os.path.join(
                    env.srcdir, os.path.dirname(env.docname),
                    self.arguments[0])
                pstake_node['texdata'] = None
            else:
                pstake_node['texpath'] = None
                pstake_node['texdata'] = "\n".join(self.content)
            pstake_node['cmdout'] = getattr(env.config, 'pstake_cmdout', None)
            pstake_node['uri'] = ''
            # Return.
            return [pstake_node]

        def _make_figure_node(self, *children):
            # Build figure node.
            if isinstance(children[0], nodes.system_message):
                return children[:1]
            figure_node = nodes.figure('', *children)
            # Pop options.
            figwidth = self.options.pop('figwidth', None)
            figclasses = self.options.pop('figclass', None)
            align = self.options.pop('align', None)
            # Figure property "figwidth".
            if figwidth:
                figure_node['width'] = figwidth
            # Figure property "figclass".
            if figclasses:
                figure_node['classes'] += figclasses
            # Figure property "align".
            if align:
                figure_node['align'] = align
            # Figure property "caption".  Only valid when texpath is used.
            if self.content:
                node = nodes.Element() # anonymous container for parsing
                self.state.nested_parse(self.content, self.content_offset, node)
                first_node = node[0]
                if isinstance(first_node, nodes.paragraph):
                    caption = nodes.caption(first_node.rawsource, '',
                                            *first_node.children)
                    caption.source = first_node.source
                    caption.line = first_node.line
                    figure_node += caption
                elif not (isinstance(first_node, nodes.comment)
                          and len(first_node) == 0):
                    error = self.state_machine.reporter.error(
                          'Figure caption must be a paragraph or empty comment.',
                          nodes.literal_block(self.block_text, self.block_text),
                          line=self.lineno)
                    return [figure_node, error]
                if len(node) > 1:
                    figure_node += nodes.legend('', *node[1:])
            return [figure_node]

        def run(self):
            self._check_align()
            results = self._make_pstake_node()
            pstake_node = results[-1]
            # If the caption (content with texpath) is available, make a figure
            # and replace the results with it.
            if self.content and pstake_node['texpath']:
                results = self._make_figure_node(*results)
            return results

    # End PstakeDirective


    def visit_pstake_node(self, node):
        # Determine a :py:class:`Filename` object.
        if node['texdata']:
            source_not_path = True
            texsource = node['texdata']
            mainname = node['targetid']
        else:
            source_not_path = False
            texsource = node['texpath']
            basename = os.path.basename(texsource)
            mainname = os.path.splitext(basename)[0]
        pngpath = os.path.join(self.builder.imgpath, mainname)
        pngpath += '.png'
        # Remove the leading relative path tokens from pngpath.
        ptokens = pngpath.split(os.sep)
        for it in range(len(ptokens)):
            if ptokens[it] != '..' and ptokens[it] != '.':
                break
        pngoutpath = os.path.join(*ptokens[it:])
        fnobj = Filename(source=texsource,
                         dest=os.path.join(self.builder.outdir, pngoutpath),
                         source_not_path=source_not_path)

        # Convert file.
        if not os.path.exists(fnobj.destdirabs):
            os.makedirs(fnobj.destdirabs)
        cmdout = open(node['cmdout'], 'a+') if node['cmdout'] else None
        Pstricks()(fnobj, cmdout=cmdout)
        if cmdout:
            cmdout.close()

        # Determine alignment.
        align = node.get('align', None)
        if align:
            self.body.append(
                '<div align="%s" class="align-%s">' % (align, align))
        else:
            self.body.append('<div>')
        self.context.append('</div>\n')

        # Append <img> tag.
        atts = dict(src=pngpath)
        # Image size.
        if 'width' in node:
            atts['width'] = node['width']
        if 'height' in node:
            atts['height'] = node['height']
        if 'scale' in node:
            for att_name in 'width', 'height':
                if att_name in atts:
                    match = re.match(r'([0-9.]+)(\S*)$', atts[att_name])
                    assert match
                    atts[att_name] = '%s%s' % (
                        float(match.group(1)) * (float(node['scale']) / 100),
                        match.group(2))
        style = []
        for att_name in 'width', 'height':
            if att_name in atts:
                if re.match(r'^[0-9.]+$', atts[att_name]):
                    # Interpret unitless values as pixels.
                    atts[att_name] += 'px'
                style.append('%s: %s;' % (att_name, atts[att_name]))
                del atts[att_name]
        # Local style sheet.
        if style:
            atts['style'] = ' '.join(style)
        # Alternate.
        if 'alt' in node:
            atts['alt'] = node['alt']
        self.body.append(self.starttag(node, 'img', '', empty=True, **atts))


    def depart_pstake_node(self, node):
        # Pop </div>.
        self.body.append(self.context.pop())


    def setup(app):
        """
        Setup function for extending Sphinx.
        """
        app.add_config_value('pstake_cmdout', None, '')

        app.add_node(pstake,
                     html=(visit_pstake_node, depart_pstake_node))

        app.add_directive('pstake', PstakeDirective)

    def setup_module():
        """
        Have nose use this method instead of :py:meth:`setup`.
        """
        pass
# End Sphinx code.

if __name__ == '__main__':
    sys.exit(main())

# vim: set et sw=4 ts=4 tw=79:
