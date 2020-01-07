"""
At step 3, we structure the script into a package.
"""


__all__ = [
    'count_line',
]


from ._core import count_line
from ._core import main  # noqa: F401
