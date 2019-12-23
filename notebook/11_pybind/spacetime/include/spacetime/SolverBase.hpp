#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::x(bool odd_plane) const
{
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).x(); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::xctr(bool odd_plane) const
{
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).xctr(); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::get_so0p(size_t iv, bool odd_plane) const
{
    if (iv >= m_field.nvar()) { throw std::out_of_range("get_so0p(): out of nvar range"); }
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).so0p(iv); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::get_so0(size_t iv, bool odd_plane) const
{
    if (iv >= m_field.nvar()) { throw std::out_of_range("get_so0(): out of nvar range"); }
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).so0(iv); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::get_so1(size_t iv, bool odd_plane) const
{
    if (iv >= m_field.nvar()) { throw std::out_of_range("get_so1(): out of nvar range"); }
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).so1(iv); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline void
SolverBase<ST,CE,SE>::set_so0(size_t iv, typename SolverBase<ST,CE,SE>::array_type const & arr, bool odd_plane)
{
    if (iv >= m_field.nvar()) { throw std::out_of_range("set_so0(): out of nvar range"); }
    if (1 != arr.shape().size()) { throw std::out_of_range("set_so0(): input not 1D"); }
    const index_type nselm = grid().nselm() - odd_plane;
    if (nselm != arr.size()) { throw std::out_of_range("set_so0(): input wrong size"); }
    for (index_type it=0; it<nselm; ++it) { selm(it, odd_plane).so0(iv) = arr[it]; }
}

template< typename ST, typename CE, typename SE >
inline void
SolverBase<ST,CE,SE>::set_so1(size_t iv, typename SolverBase<ST,CE,SE>::array_type const & arr, bool odd_plane)
{
    if (iv >= m_field.nvar()) { throw std::out_of_range("set_so1(): out of nvar range"); }
    if (1 != arr.shape().size()) { throw std::out_of_range("set_so1(): input not 1D"); }
    const index_type nselm = grid().nselm() - odd_plane;
    if (nselm != arr.size()) { throw std::out_of_range("set_so1(): input wrong size"); }
    for (index_type it=0; it<nselm; ++it) { selm(it, odd_plane).so1(iv) = arr[it]; }
}

template< typename ST, typename CE, typename SE >
inline typename SolverBase<ST,CE,SE>::array_type
SolverBase<ST,CE,SE>::get_cfl(bool odd_plane) const
{
    const index_type nselm = grid().nselm() - odd_plane;
    array_type ret(std::vector<size_t>{nselm});
    for (index_type it=0; it<nselm; ++it) { ret[it] = selm(it, odd_plane).cfl(); }
    return ret;
}

template< typename ST, typename CE, typename SE >
inline void
SolverBase<ST,CE,SE>::set_cfl(typename SolverBase<ST,CE,SE>::array_type const & arr, bool odd_plane)
{
    if (1 != arr.shape().size()) { throw std::out_of_range("set_so1(): input not 1D"); }
    const index_type nselm = grid().nselm() - odd_plane;
    if (nselm != arr.size()) { throw std::out_of_range("set_so1(): input wrong size"); }
    for (index_type it=0; it<nselm; ++it) { selm(it, odd_plane).cfl() = arr[it]; }
}

template< typename ST, typename CE, typename SE >
inline void SolverBase<ST,CE,SE>::march_half_so0(bool odd_plane)
{
    const sindex_type start = odd_plane ? -1 : 0;
    const sindex_type stop = grid().ncelm();
    for (sindex_type ic=start; ic<stop; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so0(0) = ce.calc_so0(0);
    }
}

template< typename ST, typename CE, typename SE >
inline void SolverBase<ST,CE,SE>::update_cfl(bool odd_plane)
{
    const sindex_type start = odd_plane ? -1 : 0;
    const sindex_type stop = grid().nselm();
    for (sindex_type ic=start; ic<stop; ++ic)
    {
        selm(ic, odd_plane).update_cfl();
    }
}

template< typename ST, typename CE, typename SE >
template< size_t ALPHA >
inline void SolverBase<ST,CE,SE>::march_half_so1_alpha(bool odd_plane)
{
    const sindex_type start = odd_plane ? -1 : 0;
    const sindex_type stop = grid().ncelm();
    for (sindex_type ic=start; ic<stop; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so1(0) = ce.template calc_so1_alpha<ALPHA>(0);
    }
}

template< typename ST, typename CE, typename SE >
inline void SolverBase<ST,CE,SE>::treat_boundary_so0()
{
    SE const selm_left_in = selm(0, true);
    SE       selm_left_out = selm(-1, true);
    SE const selm_right_in = selm(grid().ncelm()-1, true);
    SE       selm_right_out = selm(grid().ncelm(), true);

    selm_left_out.so0(0) = selm_right_in.so0(0);
    selm_right_out.so0(0) = selm_left_in.so0(0);
}

template< typename ST, typename CE, typename SE >
inline void SolverBase<ST,CE,SE>::treat_boundary_so1()
{
    SE const selm_left_in = selm(0, true);
    SE       selm_left_out = selm(-1, true);
    SE const selm_right_in = selm(grid().ncelm()-1, true);
    SE       selm_right_out = selm(grid().ncelm(), true);

    selm_left_out.so1(0) = selm_right_in.so1(0);
    selm_right_out.so1(0) = selm_left_in.so1(0);
}

template< typename ST, typename CE, typename SE >
template< size_t ALPHA >
inline void SolverBase<ST,CE,SE>::march_half1_alpha()
{
    march_half_so0(false);
    treat_boundary_so0();
    update_cfl(true);
    march_half_so1_alpha<ALPHA>(false);
    treat_boundary_so1();
}

template< typename ST, typename CE, typename SE >
template< size_t ALPHA >
inline void SolverBase<ST,CE,SE>::march_half2_alpha()
{
    // In the second half step, no treating boundary conditions.
    march_half_so0(true);
    update_cfl(false);
    march_half_so1_alpha<ALPHA>(true);
}

template< typename ST, typename CE, typename SE >
template <size_t ALPHA>
inline void SolverBase<ST,CE,SE>::march_alpha(size_t steps)
{
    for (size_t it=0; it<steps; ++it)
    {
        march_half1_alpha<ALPHA>();
        march_half2_alpha<ALPHA>();
    }
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
