#include "fma.hpp"

#include <immintrin.h>

void multiply_and_add_simd(float* a, float* b, float* c, float* d)
{
    for (size_t i=0; i<repeat; ++i)
    {
        __m256 * ma = (__m256 *) (&a[i*width]);
        __m256 * mb = (__m256 *) (&b[i*width]);
        __m256 * mc = (__m256 *) (&c[i*width]);
        __m256 * md = (__m256 *) (&d[i*width]);
        *md = _mm256_fmadd_ps(*ma, *mb, *mc);
    }
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
