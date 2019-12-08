# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING


"""
A library for one-dimensional space-time conservation element and solution
element (CESE) method
"""


import sys
import collections

from . import _core
from . import _pstake as _pst


class PstCanvas:

    def __init__(self, unit=None, padding=0, extent=None, show_grid=False):

        self.unit = unit if unit else "1cm"

        if isinstance(padding, collections.Sequence):
            if 2 == len(padding):
                padding = padding[0], padding[1], padding[0], padding[1]
            elif 4 == len(padding):
                padding = tuple(padding)
            else:
                raise ValueError('invalid padding length: %s' % padding)
        else:
            padding = tuple([padding]*4)
        self.padding = padding

        if isinstance(extent, collections.Sequence):
            if 2 == len(extent):
                extent = extent[0], extent[1], extent[0], extent[1]
            elif 4 == len(extent):
                extent = tuple(extent)
            else:
                raise ValueError('invalid extent length: %s' % extent)
        elif extent is not None:
            extent = tuple([extent]*4)
        self._extent = extent

        self._cmdbody = list()

        self.xmin = sys.float_info.max
        self.xmax = sys.float_info.min
        self.ymin = sys.float_info.max
        self.ymax = sys.float_info.min

        self.show_grid = show_grid

    @property
    def extent(self):

        if self._extent:
            return self._extent
        else:
            return (self.xmin, self.ymin, self.xmax, self.ymax)

    @property
    def padded_extent(self):

        xmin, ymin, xmax, ymax = self.extent
        pxmin, pymin, pxmax, pymax = self.padding
        return (
            xmin - pxmin, ymin - pymin,
            xmax + pxmax, ymax + pymax,
        )

    def to_filename(self, filename):

        runner = _pst.Pstricks()
        fn = _pst.Filename(source=str(self), dest=filename,
                           source_not_path=True)
        runner(fn)

    def __str__(self):

        clist = [self.set(unit=self.unit, append=False)]
        xmin, ymin, xmax, ymax = self.padded_extent
        clist.append(r'\begin{pspicture}(%g,%g)(%g,%g)' % (
            xmin, ymin, xmax, ymax))
        if self.show_grid:
            clist.append(r'\psgrid(0,0)(%g,%g)(%g,%g)' % (
                xmin, ymin, xmax, ymax))
        clist.append('')

        clist.extend(self._cmdbody)

        clist.append('')
        clist.append(r'\end{pspicture}')
        return '\n'.join(clist)

    def append(self, cmd, *args, **kw):

        meth = getattr(self, cmd)
        cmd = meth(*args, **kw)
        self._cmdbody.append(cmd)

    def append_blank(self):

        self._cmdbody.append('')

    def _points(self, *pts):

        x = [p[0] for p in pts]
        self.xmin = min(self.xmin, min(x))
        self.xmax = max(self.xmax, max(x))
        y = [p[1] for p in pts]
        self.ymin = min(self.ymin, min(y))
        self.ymax = max(self.ymax, max(y))
        return "".join("(%g,%g)" % (p[0], p[1]) for p in pts)

    @staticmethod
    def _options(**kw):

        return ",".join("%s=%s" % (k, kw[k]) for k in kw)

    def set(self, **kw):

        append = kw.pop('append', True)
        cmd = r"\psset{%s}" % self._options(**kw)
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def _crdcmd(self, name, *args, **kw):

        append = kw.pop('append', True)
        o = self._options(**kw)
        if o:
            o = '[%s]' % o
        cmd = r"\%s%s%s" % (name, o, self._points(*args))
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def frame(self, p0, p1, **kw):

        return self._crdcmd("psframe", p0, p1, **kw)

    def line(self, *args, **kw):

        return self._crdcmd("psline", *args, **kw)
        append = kw.pop('append', True)
        arrows = kw.pop('arrows', '')
        if arrows: arrows = '{%s}' % arrows
        o = self._options(**kw)
        if o:
            o = '[%s]' % o
        cmd = r"\%s%s%s%s" % (name, o, arrows, self._points(*args))
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def arc(self, pt, radius, angle_a, angle_b, **kw):

        arrows = kw.pop('arrows', '')
        arrows = '{%s}' % arrows if arrows else arrows
        append = kw.pop('append', True)
        o = self._options(**kw)
        if o:
            o = '[%s]' % o
        cmd = r"\psarc%s%s%s{%g}{%g}{%g}" % (
            o, arrows, self._points(pt), radius, angle_a, angle_b)
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def brace(self, p0, p1, text='', **kw):

        append = kw.pop('append', True)
        o = self._options(**kw)
        if o:
            o = '[%s]' % o
        if text:
            text = '{%s}' % text
        cmd = r"\psbrace%s%s%s%s" % (
            o, self._points(p0), self._points(p1), text)
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def dots(self, *args, **kw):

        return self._crdcmd("psdots", *args, **kw)

    def rput(self, rot, crd, text, **kw):

        append = kw.pop('append', True)
        cmd = r"\rput[%s](%g,%g){%s}" % (rot, crd[0], crd[1], text)
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def uput(self, sep, rot, crd, text, **kw):

        append = kw.pop('append', True)
        if not isinstance(sep, str):
            sep = '%g' % sep
        cmd = r"\uput{%s}[%s](%g,%g){%s}" % (sep, rot, crd[0], crd[1], text)
        if append:
            self._cmdbody.append(cmd)
        return cmd

    def selm(self, se, time, **kw):

        dt = kw.pop('dt', se.dt)
        x, xn, xp = se.x, se.xneg, se.xpos
        y, yn, yp = time, time-dt/2, time+dt/2
        xctr = se.xctr

        append = kw.pop('append', True)

        sep = kw.pop('sep', 0.05)
        sep *= dt
        sep2 = sep * 0.5
        sep3 = sep * 0.75
        sep4 = sep2 + sep3

        options = dict(linewidth='1pt')
        options.update(append=False, **kw)
        kw = options

        cmds = list()
        cmds.append(self.dots((xctr, y), **kw))
        # Upside.
        cmds.append(self.line((x+sep2,y+sep3), (x+sep2,yp-sep4), **kw))
        cmds.append(self.line((x-sep2,y+sep3), (x-sep2,yp-sep4), **kw))
        cmds.append(self.arc((x, yp-sep), sep2, 0, 180, **kw))
        # Downside.
        cmds.append(self.line((x+sep2,y-sep3), (x+sep2,yn+sep4), **kw))
        cmds.append(self.line((x-sep2,y-sep3), (x-sep2,yn+sep4), **kw))
        cmds.append(self.arc((x, yn+sep), sep2, 180, 0, **kw))
        # Rightside.
        cmds.append(self.line((x+sep3,y+sep2), (xp-sep4,y+sep2), **kw))
        cmds.append(self.line((x+sep3,y-sep2), (xp-sep4,y-sep2), **kw))
        cmds.append(self.arc((xp-sep, y), sep2, 270, 90, **kw))
        # Leftside.
        cmds.append(self.line((x-sep3,y+sep2), (xn+sep4,y+sep2), **kw))
        cmds.append(self.line((x-sep3,y-sep2), (xn+sep4,y-sep2), **kw))
        cmds.append(self.arc((xn+sep, y), sep2, 90, 270, **kw))
        # Pack up.
        cmd = '\n'.join(cmds)

        if append:
            self._cmdbody.append(cmd)
        return cmd

# vim: set et sw=4 ts=4:
