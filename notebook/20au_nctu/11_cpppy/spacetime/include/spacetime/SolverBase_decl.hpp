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
#include "spacetime/Field_decl.hpp"

namespace spacetime
{

class Selm;

/**
 * Algorithmic definition for solution.  It holds the type information for the
 * CE and SE.
 */
template< typename ST, typename CE, typename SE >
class SolverBase
  : public std::enable_shared_from_this<ST>
{

public:

    using value_type = Field::value_type;
    using array_type = Field::array_type;
    using celm_type = CE;
    using selm_type = SE;

protected:

    class ctor_passkey {};

    template<class ... Args> static std::shared_ptr<ST> construct_impl(Args&& ... args)
    {
        return std::make_shared<ST>(std::forward<Args>(args) ..., ctor_passkey());
    }

public:

    std::shared_ptr<ST> clone(bool grid=false)
    {
        /* The only purpose of this reinterpret_cast is to workaround for
         * static polymorphism. */
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto ret = std::make_shared<ST>(*reinterpret_cast<ST*>(this));
        if (grid)
        {
            std::shared_ptr<Grid> new_grid = m_field.clone_grid();
            ret->m_field.set_grid(new_grid);
        }
        return ret;
    }

    SolverBase(
        std::shared_ptr<Grid> const & grid
      , value_type time_increment
      , size_t nvar
      , ctor_passkey const &
    )
      : m_field(grid, time_increment, nvar)
    {}

    SolverBase() = delete;
    SolverBase(SolverBase const & ) = default;
    SolverBase(SolverBase       &&) = default;
    SolverBase & operator=(SolverBase const & ) = default;
    SolverBase & operator=(SolverBase       &&) = default;
    ~SolverBase() = default;

    Grid const & grid() const { return m_field.grid(); }
    Grid       & grid()       { return m_field.grid(); }

    array_type x(bool odd_plane) const;
    array_type xctr(bool odd_plane) const;

#define DECL_ST_ARRAY_ACCESS_0D(NAME) \
    array_type const & NAME() const { return m_field.NAME(); } \
    array_type       & NAME()       { return m_field.NAME(); } \
    array_type get_ ## NAME(bool odd_plane) const; \
    void set_ ## NAME(array_type const & arr, bool odd_plane);
#define DECL_ST_ARRAY_ACCESS_1D(NAME) \
    array_type const & NAME() const { return m_field.NAME(); } \
    array_type       & NAME()       { return m_field.NAME(); } \
    array_type get_ ## NAME(size_t iv, bool odd_plane) const; \
    void set_ ## NAME(size_t iv, array_type const & arr, bool odd_plane);

    DECL_ST_ARRAY_ACCESS_0D(cfl)
    DECL_ST_ARRAY_ACCESS_1D(so0)
    DECL_ST_ARRAY_ACCESS_1D(so1)

#undef DECL_ST_ARRAY_ACCESS_1D
#undef DECL_ST_ARRAY_ACCESS_0D

    array_type get_so0p(size_t iv, bool odd_plane) const;

    size_t nvar() const { return m_field.nvar(); }

    void set_time_increment(value_type time_increment) { m_field.set_time_increment(time_increment); }

    real_type time_increment() const { return m_field.time_increment(); }
    real_type dt() const { return m_field.dt(); }
    real_type hdt() const { return m_field.hdt(); }
    real_type qdt() const { return m_field.qdt(); }

    // NOLINTNEXTLINE(readability-const-return-type)
    CE const celm(sindex_type ielm, bool odd_plane) const { return m_field.celm<CE>(ielm, odd_plane); }
    CE       celm(sindex_type ielm, bool odd_plane)       { return m_field.celm<CE>(ielm, odd_plane); }
    // NOLINTNEXTLINE(readability-const-return-type)
    CE const celm_at(sindex_type ielm, bool odd_plane) const { return m_field.celm_at<CE>(ielm, odd_plane); }
    CE       celm_at(sindex_type ielm, bool odd_plane)       { return m_field.celm_at<CE>(ielm, odd_plane); }

    // NOLINTNEXTLINE(readability-const-return-type)
    SE const selm(sindex_type ielm, bool odd_plane) const { return m_field.selm<SE>(ielm, odd_plane); }
    SE       selm(sindex_type ielm, bool odd_plane)       { return m_field.selm<SE>(ielm, odd_plane); }
    // NOLINTNEXTLINE(readability-const-return-type)
    SE const selm_at(sindex_type ielm, bool odd_plane) const { return m_field.selm_at<SE>(ielm, odd_plane); }
    SE       selm_at(sindex_type ielm, bool odd_plane)       { return m_field.selm_at<SE>(ielm, odd_plane); }

    void update_cfl(bool odd_plane);
    void march_half_so0(bool odd_plane);
    template <size_t ALPHA> void march_half_so1_alpha(bool odd_plane);
    void treat_boundary_so0();
    void treat_boundary_so1();

    void setup_march() { update_cfl(false); }
    template <size_t ALPHA> void march_half1_alpha();
    template <size_t ALPHA> void march_half2_alpha();
    template <size_t ALPHA> void march_alpha(size_t steps);

private:

    Field m_field;

}; /* end class SolverBase */

} /* end namespace spacetime */

#define SPACETIME_DERIVED_SELM_BODY_DEFAULT \
public: \
    using base_type = Selm; \
    using base_type::base_type; \
    value_type xn(size_t iv) const; \
    value_type xp(size_t iv) const; \
    value_type tn(size_t iv) const; \
    value_type tp(size_t iv) const; \
    value_type so0p(size_t iv) const; \
    value_type & update_cfl();

/* vim: set et ts=4 sw=4: */
