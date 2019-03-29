"""
At step 0, it is unclear how to expose the interface for this module.
"""


__all__ = [
    'count_line',
]


import sys
import os
import os.path


def count_line(fname):
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))


def main():
    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif 2 == len(sys.argv):
        count_line(sys.argv[1])
    else:
        sys.stdout.write('only one argument is allowed\n')


if __name__ == '__main__':
    main()
