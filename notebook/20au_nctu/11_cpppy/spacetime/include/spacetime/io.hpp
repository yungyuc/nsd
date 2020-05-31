#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <iostream>

#include "spacetime/Grid.hpp"
#include "spacetime/Celm.hpp"
#include "spacetime/Solver.hpp"
#include "spacetime/Selm.hpp"
#include "spacetime/kernel/linear_scalar.hpp"
#include "spacetime/kernel/inviscid_burgers.hpp"

namespace spacetime
{

inline
std::ostream& operator<<(std::ostream& os, const Grid & grid)
{
    os << "Grid(xmin=" << grid.xmin() << ", xmax=" << grid.xmax() << ", ncelm=" << grid.ncelm() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const Field & sol)
{
    os << "Field(grid=" << sol.grid() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const Solver & sol)
{
    os << "Solver(grid=" << sol.grid() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const Celm & elm)
{
    os << "Celm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const Selm & elm)
{
    os << "Selm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const InviscidBurgersSolver & sol)
{
    os << "InviscidBurgersSolver(grid=" << sol.grid() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const InviscidBurgersCelm & elm)
{
    os << "InviscidBurgersCelm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const InviscidBurgersSelm & elm)
{
    os << "InviscidBurgersSelm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const LinearScalarSolver & sol)
{
    os << "LinearScalarSolver(grid=" << sol.grid() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const LinearScalarCelm & elm)
{
    os << "LinearScalarCelm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const LinearScalarSelm & elm)
{
    os << "LinearScalarSelm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
