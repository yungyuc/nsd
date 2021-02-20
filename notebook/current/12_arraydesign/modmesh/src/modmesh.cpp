/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD-style license; see COPYING
 */

#include "modmesh/python/python.hpp" // Must be the first include.
#include "modmesh/modmesh.hpp"

namespace modmesh
{

namespace python
{

void initialize(pybind11::module & mod)
{

    WrapStaticGrid1d::commit(mod);
    WrapStaticGrid2d::commit(mod);
    WrapStaticGrid3d::commit(mod);
    WrapTimeRegistry::commit(mod);
    mod.attr("time_registry") = mod.attr("TimeRegistry").attr("me");

}

} /* end namespace python */

} /* end namespace modmesh */

PYBIND11_MODULE(_modmesh, mod)
{
    modmesh::python::initialize(mod);
}

// vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4 sts=4:
