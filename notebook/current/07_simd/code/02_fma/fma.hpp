#pragma once

#include "StopWatch.hpp"

#include <cstddef>

constexpr const size_t width = 8;
constexpr const size_t repeat = 1024 * 1024;
constexpr const size_t nelem = width * repeat;

constexpr const size_t necount = 20;
constexpr const float error_tolerance = 2.e-7;

void multiply_and_add_loop(float* a, float* b, float* c, float* d);

void multiply_and_add_simd(float* a, float* b, float* c, float* d);

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
