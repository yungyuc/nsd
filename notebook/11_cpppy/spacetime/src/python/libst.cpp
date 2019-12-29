/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python.hpp" // must be first

#include "spacetime.hpp"

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

namespace
{

PyObject * initialize_spacetime(pybind11::module * mod)
{
    namespace spy = spacetime::python;
    xt::import_numpy(); // otherwise numpy c api segfault.
    mod->doc() = "_libst: One-dimensional space-time CESE method code";
    spy::WrapGrid::commit(mod, "Grid", "Spatial grid data");
    spy::WrapField::commit(mod, "Field", "Solution data");
    return mod->ptr();
}

} /* end namespace */

PYBIND11_MODULE(_libst, mod) // NOLINT
{
    namespace spy = spacetime::python;
    spy::ModuleInitializer::get_instance()
        .add(initialize_spacetime)
        .add_solver<spy::WrapSolver, spy::WrapCelm, spy::WrapSelm>
        (&mod, "", "no equation")
        .add_solver<spy::WrapLinearScalarSolver, spy::WrapLinearScalarCelm, spy::WrapLinearScalarSelm>
        (&mod, "LinearScalar", "a linear scalar equation")
        .add_solver<spy::WrapInviscidBurgersSolver, spy::WrapInviscidBurgersCelm, spy::WrapInviscidBurgersSelm>
        (&mod, "InviscidBurgers", "the inviscid Burgers equation")
        .initialize(&mod)
    ;
}

// vim: set et sw=4 ts=4:
