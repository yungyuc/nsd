# Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
# BSD-style license; see COPYING

"""
General mesh data definition and manipulation in one, two, and
three-dimensional space.
"""

from . import _modmesh


__all__ = [
    'TimeRegistry',
    'time_registry',
    'StaticGrid1d',
    'StaticGrid2d',
    'StaticGrid3d',
]


TimeRegistry = _modmesh.TimeRegistry
time_registry = _modmesh.time_registry

StaticGrid1d = _modmesh.StaticGrid1d
StaticGrid2d = _modmesh.StaticGrid2d
StaticGrid3d = _modmesh.StaticGrid3d

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
