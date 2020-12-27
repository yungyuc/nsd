#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD-style license; see COPYING
 */

// Used in this file.
#include <cstdint>

// Shared by all code.
#include <algorithm>
#include <memory>
#include <iostream>
#include <sstream>
#include <map>

#define MODMESH_EXCEPT(CLS, EXC, MSG) throw EXC(#CLS ": " MSG);

namespace modmesh
{

/**
 * Spatial table basic information.  Any table-based data store for spatial
 * data should inherit this class template.
 */
template <size_t ND>
class SpaceBase
{

public:

    static constexpr const size_t NDIM = ND;

    using serial_type = uint32_t;
    using real_type = double;

}; /* end class SpaceBase */

} /* end namespace modmesh */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
