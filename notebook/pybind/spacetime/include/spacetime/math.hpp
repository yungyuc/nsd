#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <type_traits>

namespace spacetime
{

namespace detail
{

template< typename T >
inline constexpr T pow(T /*base*/, std::integral_constant<size_t, 0> /*unused*/) { return 1; }

template< typename T >
inline constexpr T pow(T base, std::integral_constant<size_t, 1> /*unused*/) { return base; }

template< typename T, size_t N >
inline constexpr T pow(T base, std::integral_constant<size_t, N> /*unused*/)
{
    return pow(base, std::integral_constant<size_t, N-1>()) * base;
}

} /* end namespace detail */

template< size_t N, typename T >
inline constexpr T pow(T base)
{
    return detail::pow(base, std::integral_constant<size_t, N>());
}

} /* end namespace spacetime */