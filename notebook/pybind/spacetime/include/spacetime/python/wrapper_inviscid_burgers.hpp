#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python/common.hpp"

namespace spacetime
{

namespace python
{

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapInviscidBurgersSolver
  : public WrapSolverBase< WrapInviscidBurgersSolver, InviscidBurgersSolver >
{

    using base_type = WrapSolverBase< WrapInviscidBurgersSolver, InviscidBurgersSolver >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapInviscidBurgersSolver(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def(
                py::init(static_cast<std::shared_ptr<wrapped_type> (*) (
                    std::shared_ptr<Grid> const &, typename wrapped_type::value_type
                )>(&wrapped_type::construct))
              , py::arg("grid"), py::arg("time_increment")
            )
        ;
    }

}; /* end class WrapInviscidBurgersSolver */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapInviscidBurgersCelm
  : public WrapCelmBase< WrapInviscidBurgersCelm, InviscidBurgersCelm >
{

    using base_type = WrapCelmBase< WrapInviscidBurgersCelm, InviscidBurgersCelm >;
    friend base_type::base_type::base_type;

    WrapInviscidBurgersCelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapInviscidBurgersCelm */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapInviscidBurgersSelm
  : public WrapSelmBase< WrapInviscidBurgersSelm, InviscidBurgersSelm >
{

    using base_type = WrapSelmBase< WrapInviscidBurgersSelm, InviscidBurgersSelm >;
    friend base_type::base_type::base_type;

    WrapInviscidBurgersSelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapInviscidBurgersSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
