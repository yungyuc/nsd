#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Inviscid Burgers' equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

/**
 * Flux calculator for the solution element for the inviscid Burgers equation.
 */
class InviscidBurgersSelm
  : public Selm
{
    SPACETIME_DERIVED_SELM_BODY_DEFAULT
}; /* end class FelmBase */

using InviscidBurgersCelm = CelmBase<InviscidBurgersSelm>;

class InviscidBurgersSolver
  : public SolverBase<InviscidBurgersSolver, InviscidBurgersCelm, InviscidBurgersSelm>
{

public:

    using base_type = SolverBase<InviscidBurgersSolver, InviscidBurgersCelm, InviscidBurgersSelm>;
    using base_type::base_type;

    static std::shared_ptr<InviscidBurgersSolver>
    construct(std::shared_ptr<Grid> const & grid, value_type time_increment)
    {
        return construct_impl(grid, time_increment, 1);
    }

}; /* end class InviscidBurgersSolver */

/**
 * Flux for the negative branch on the x-plane. (Flux direction in forward t.)
 */
inline
InviscidBurgersSelm::value_type InviscidBurgersSelm::xn(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xneg()) - xctr();
    return dxneg() * (so0(iv) + displacement * so1(iv));
}

/**
 * Flux for the positive branch on the x-plane. (Flux direction in forward t.)
 */
inline
InviscidBurgersSelm::value_type InviscidBurgersSelm::xp(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xpos()) - xctr();
    return dxpos() * (so0(iv) + displacement * so1(iv));
}

/**
 * Flux for the backward (behind) branch on the t-plane. (Flux direction in positive x.)
 */
inline
InviscidBurgersSelm::value_type InviscidBurgersSelm::tn(size_t iv) const
{
    const value_type displacement = x() - xctr();
    const value_type u = so0(iv);
    const value_type u_2 = u * u;
    value_type ret = 0.5 * u_2; /* f(u) */
    ret += displacement * u * so1(iv); /* displacement in x */
    ret += qdt() * u_2 * so1(iv); /* displacement in t */
    return hdt() * ret;
}

/**
 * Flux for the forward (ahead) branch on the t-plane. (Flux direction in positive x.)
 */
inline
InviscidBurgersSelm::value_type InviscidBurgersSelm::tp(size_t iv) const
{
    const value_type displacement = x() - xctr();
    const value_type u = so0(iv);
    const value_type u_2 = u * u;
    value_type ret = 0.5 * u_2; /* f(u) */
    ret += displacement * u * so1(iv); /* displacement in x */
    ret -= qdt() * u_2 * so1(iv); /* displacement in t */
    return hdt() * ret;
}

/**
 * Approximated value of the solution variable at the t+ tip of the solution element.
 */
inline
InviscidBurgersSelm::value_type InviscidBurgersSelm::so0p(size_t iv) const
{
    value_type ret = so0(iv);
    ret += (x()-xctr()) * so1(iv); /* displacement in x */
    ret -= hdt() * so1(iv); /* displacement in t */
    return ret;
}

inline
InviscidBurgersSelm::value_type & InviscidBurgersSelm::update_cfl()
{
    const value_type hdx = std::min(dxneg(), dxpos());
    this->cfl() = std::fabs(so0(0)) * field().hdt() / hdx;
    return this->cfl();
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
