#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

inline
void Selm::move_at(sindex_type offset)
{
    const size_t xindex = this->xindex() + offset;
    if (xindex < 1 || xindex >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Selm(xindex=" << this->xindex() << ")::move_at(offset=" << offset
            << "): xindex = " << xindex
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    move(offset);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
