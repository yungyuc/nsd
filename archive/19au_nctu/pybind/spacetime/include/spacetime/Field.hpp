#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Field_decl.hpp"
#include "spacetime/Celm_decl.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

inline
Field::Field(std::shared_ptr<Grid> const & grid, Field::value_type time_increment, size_t nvar)
  : m_grid(grid)
  , m_so0(array_type(std::vector<size_t>{grid->xsize(), nvar}))
  , m_so1(array_type(std::vector<size_t>{grid->xsize(), nvar}))
  , m_cfl(array_type(std::vector<size_t>{grid->xsize()}))
{
    set_time_increment(time_increment);
}

inline
void Field::set_time_increment(value_type time_increment)
{
    m_time_increment = time_increment;
    m_half_time_increment = 0.5 * time_increment;
    m_quarter_time_increment = 0.25 * time_increment;
}

template< typename CE >
// NOLINTNEXTLINE(readability-const-return-type)
inline
CE const Field::celm_at(sindex_type ielm, bool odd_plane) const
{
    const CE elm = celm<CE>(ielm, odd_plane);
    if (elm.xindex() < 2 || elm.xindex() >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Field::celm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    return elm;
}

template< typename CE >
inline
CE Field::celm_at(sindex_type ielm, bool odd_plane)
{
    const CE elm = celm<CE>(ielm, odd_plane);
    if (elm.xindex() < 2 || elm.xindex() >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Field::celm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    return elm;
}

template< typename SE >
// NOLINTNEXTLINE(readability-const-return-type)
inline
SE const Field::selm_at(sindex_type ielm, bool odd_plane) const
{
    const SE elm = selm<SE>(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Field::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

template< typename SE >
inline
SE Field::selm_at(sindex_type ielm, bool odd_plane)
{
    const SE elm = selm<SE>(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Field::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
