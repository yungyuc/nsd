#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Linear scalar equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

class LinearScalarSelm
  : public Selm
{
    SPACETIME_DERIVED_SELM_BODY_DEFAULT
}; /* end class LinearScalarSelm */

using LinearScalarCelm = CelmBase<LinearScalarSelm>;

class LinearScalarSolver
  : public SolverBase<LinearScalarSolver, LinearScalarCelm, LinearScalarSelm>
{

public:

    using base_type = SolverBase<LinearScalarSolver, LinearScalarCelm, LinearScalarSelm>;
    using base_type::base_type;

    static std::shared_ptr<LinearScalarSolver>
    construct(std::shared_ptr<Grid> const & grid, value_type time_increment)
    {
        return construct_impl(grid, time_increment, 1);
    }

}; /* end class LinearScalarSolver */

inline
LinearScalarSelm::value_type LinearScalarSelm::xn(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xneg()) - xctr();
    return dxneg() * (so0(iv) + displacement * so1(iv));
}

inline
LinearScalarSelm::value_type LinearScalarSelm::xp(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xpos()) - xctr();
    return dxpos() * (so0(iv) + displacement * so1(iv));
}

inline
LinearScalarSelm::value_type LinearScalarSelm::tn(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret += qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

inline
LinearScalarSelm::value_type LinearScalarSelm::tp(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret -= qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

inline
LinearScalarSelm::value_type LinearScalarSelm::so0p(size_t iv) const
{
    value_type ret = so0(iv);
    ret += (x()-xctr()) * so1(iv); /* displacement in x */
    ret -= hdt() * so1(iv); /* displacement in t */
    return ret;
}

inline
LinearScalarSelm::value_type & LinearScalarSelm::update_cfl()
{
    const value_type hdx = std::min(dxneg(), dxpos());
    this->cfl() = field().hdt() / hdx;
    return this->cfl();
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
