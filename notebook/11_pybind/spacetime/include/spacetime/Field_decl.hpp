#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <memory>
#include <vector>

#include "xtensor/xarray.hpp"
#include "xtensor/xfixed.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/Grid_decl.hpp"

namespace spacetime
{

class Celm;
class Selm;

/**
 * Data class for solution.  It doesn't contain type information for the CE and
 * SE.  A Field declared as const is useless.
 */
class Field
{

public:

    using value_type = Grid::value_type;
    using array_type = Grid::array_type;

    Field(std::shared_ptr<Grid> const & grid, value_type time_increment, size_t nvar);

    Field() = delete;
    Field(Field const & ) = default;
    Field(Field       &&) = default;
    Field & operator=(Field const & ) = default;
    Field & operator=(Field       &&) = default;
    ~Field() = default;

    Field clone(bool grid=false) const
    {
        Field ret(*this);
        if (grid) { ret.m_grid = clone_grid(); }
        return ret;
    }

    std::shared_ptr<Grid> clone_grid() const
    {
        return m_grid->clone();
    }

    void set_grid(std::shared_ptr<Grid> const & grid) { m_grid = grid; }

    Grid const & grid() const { return *m_grid; }
    Grid       & grid()       { return *m_grid; }

    array_type const & so0() const { return m_so0; }
    array_type       & so0()       { return m_so0; }
    array_type const & so1() const { return m_so1; }
    array_type       & so1()       { return m_so1; }
    array_type const & cfl() const { return m_cfl; }
    array_type       & cfl()       { return m_cfl; }

    value_type const & so0(size_t it, size_t iv) const { return m_so0(it, iv); }
    value_type       & so0(size_t it, size_t iv)       { return m_so0(it, iv); }
    value_type const & so1(size_t it, size_t iv) const { return m_so1(it, iv); }
    value_type       & so1(size_t it, size_t iv)       { return m_so1(it, iv); }
    value_type const & cfl(size_t it) const { return m_cfl(it); }
    value_type       & cfl(size_t it)       { return m_cfl(it); }

    size_t nvar() const { return m_so0.shape()[1]; }

    void set_time_increment(value_type time_increment);

    real_type time_increment() const { return m_time_increment; }
    real_type dt() const { return m_time_increment; }
    real_type hdt() const { return m_half_time_increment; }
    real_type qdt() const { return m_quarter_time_increment; }

    // NOLINTNEXTLINE(readability-const-return-type)
    template< typename CE > CE const celm(sindex_type ielm, bool odd_plane) const { return CE(this, ielm, odd_plane, typename CE::const_ctor_passkey()); }
    template< typename CE > CE       celm(sindex_type ielm, bool odd_plane)       { return CE(this, ielm, odd_plane); }
    // NOLINTNEXTLINE(readability-const-return-type)
    template< typename CE > CE const celm_at(sindex_type ielm, bool odd_plane) const;
    template< typename CE > CE       celm_at(sindex_type ielm, bool odd_plane);

    // NOLINTNEXTLINE(readability-const-return-type)
    template< typename SE > SE const selm(sindex_type ielm, bool odd_plane) const { return SE(this, ielm, odd_plane, typename SE::const_ctor_passkey()); }
    template< typename SE > SE       selm(sindex_type ielm, bool odd_plane)       { return SE(this, ielm, odd_plane); }
    // NOLINTNEXTLINE(readability-const-return-type)
    template< typename SE > SE const selm_at(sindex_type ielm, bool odd_plane) const;
    template< typename SE > SE       selm_at(sindex_type ielm, bool odd_plane);

private:

    std::shared_ptr<Grid> m_grid;

    array_type m_so0;
    array_type m_so1;
    array_type m_cfl;

    real_type m_time_increment = 0;
    // Cached value;
    real_type m_half_time_increment = 0;
    real_type m_quarter_time_increment = 0;

}; /* end class Field */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
