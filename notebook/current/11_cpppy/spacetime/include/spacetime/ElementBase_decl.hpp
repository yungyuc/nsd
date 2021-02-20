#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "xtensor/xarray.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"

namespace spacetime
{

class Grid;
class Field;

template< class ET >
class ElementBase
{

public:

    using value_type = real_type;
    using array_type = xt::xarray<value_type, xt::layout_type::row_major>;
    using base_type = ElementBase;
    using element_type = ET;

    ElementBase(Field * field, value_type * xptr) : m_field(field), m_xptr(xptr) {}

    ElementBase() = delete;

    ET duplicate() { return *static_cast<ET *>(this); }

    Grid const & grid() const;
    Field const & field() const { return *m_field; }
    Field       & field()       { return *m_field; }

    value_type time_increment() const { return field().time_increment(); }
    value_type dt() const { return field().dt(); }
    value_type hdt() const { return field().hdt(); }
    value_type qdt() const { return field().qdt(); }

    value_type x() const { return *m_xptr; }
    value_type dx() const { return xpos() - xneg(); }
    value_type xneg() const { return *(m_xptr-1); /*NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)*/ }
    value_type xpos() const { return *(m_xptr+1); /*NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)*/ }
    value_type xctr() const { return static_cast<ET const *>(this)->xctr(); }

    void move(ssize_t offset) { m_xptr += offset; /*NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)*/ }
    void move_at(ssize_t offset) { static_cast<ET *>(this)->move_at(offset); }

    void move_left() { move(-2); }
    void move_right() { move(2); }
    void move_neg() { move(-1); }
    void move_pos() { move(1); }

    void move_left_at() { move_at(-2); }
    void move_right_at() { move_at(2); }
    void move_neg_at() { move_at(-1); }
    void move_pos_at() { move_at(1); }

    bool operator==(ET const & b) const { return (m_field == b.m_field) && (m_xptr == b.m_xptr); }
    bool operator!=(ET const & b) const { return (m_field != b.m_field) || (m_xptr != b.m_xptr); }
    bool operator< (ET const & b) const { return (m_field == b.m_field) && (m_xptr <  b.m_xptr); }
    bool operator<=(ET const & b) const { return (m_field == b.m_field) && (m_xptr <= b.m_xptr); }
    bool operator> (ET const & b) const { return (m_field == b.m_field) && (m_xptr >  b.m_xptr); }
    bool operator>=(ET const & b) const { return (m_field == b.m_field) && (m_xptr >= b.m_xptr); }

protected:

    size_t xindex() const;

private:

    Field * m_field;
    value_type * m_xptr;

    friend Grid;
    friend Field;

}; /* end class ElementBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
