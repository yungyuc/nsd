#include "fma.hpp"

void multiply_and_add_loop(float* a, float* b, float* c, float* d)
{
    for (size_t i=0; i<repeat*width; i+=width)
    {
        for (size_t j=i; j<i+width; ++j)
        {
            d[j] = a[j] * b[j] + c[j];
        }
    }
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
