#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <string>

#include "spacetime/python/common.hpp"

namespace spacetime
{

namespace python
{

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapLinearScalarSolver
  : public WrapSolverBase< WrapLinearScalarSolver, LinearScalarSolver >
{

    using base_type = WrapSolverBase< WrapLinearScalarSolver, LinearScalarSolver >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapLinearScalarSolver(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def
            (
                py::init(static_cast<std::shared_ptr<wrapped_type> (*) (
                    std::shared_ptr<Grid> const &, typename wrapped_type::value_type
                )>(&wrapped_type::construct))
              , py::arg("grid"), py::arg("time_increment")
            )
        ;
    }

}; /* end class WrapLinearScalarSolver */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapLinearScalarCelm
  : public WrapCelmBase< WrapLinearScalarCelm, LinearScalarCelm >
{

    using base_type = WrapCelmBase< WrapLinearScalarCelm, LinearScalarCelm >;
    friend base_type::base_type::base_type;

    WrapLinearScalarCelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapLinearScalarCelm */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapLinearScalarSelm
  : public WrapSelmBase< WrapLinearScalarSelm, LinearScalarSelm >
{

    using base_type = WrapSelmBase< WrapLinearScalarSelm, LinearScalarSelm >;
    friend base_type::base_type::base_type;

    WrapLinearScalarSelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapLinearScalarSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
