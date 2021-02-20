#include "StopWatch.hpp"

#include <immintrin.h>

#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

#ifdef __APPLE__
// Macos hasn't implemented the C11 aligned_alloc as of the time 2019/8.
void * aligned_alloc(size_t alignment, size_t size)
{
    void * ptr;
    posix_memalign(&ptr, alignment, size);
    return ptr;
}
#endif

constexpr const size_t width = 8; // count of floating-point
constexpr const size_t repeat = 1024 * 1024;
constexpr const size_t nelem = width * repeat;

constexpr const size_t necount = 20;
constexpr const float error_tolerance = 2.e-7;

void multiply1_loop(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat*width; i+=width)
    {
        for (size_t j=i; j<i+width; ++j)
        {
            r[j] = a[j] * b[j];
        }
    }
}

void multiply1_simd(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat; ++i)
    {
        __m256 * ma = (__m256 *) (&a[i*width]);
        __m256 * mb = (__m256 *) (&b[i*width]);
        __m256 * mr = (__m256 *) (&r[i*width]);
        *mr = _mm256_mul_ps(*ma, *mb);
    }
}

void multiply3_loop(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat*width; i+=width)
    {
        for (size_t j=i; j<i+width; ++j)
        {
            r[j] = a[j] * a[j];
            r[j] *= b[j];
            r[j] *= b[j];
        }
    }
}

void multiply3_simd(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat; ++i)
    {
        __m256 * ma = (__m256 *) (&a[i*width]);
        __m256 * mb = (__m256 *) (&b[i*width]);
        __m256 * mr = (__m256 *) (&r[i*width]);
        *mr = _mm256_mul_ps(*ma, *ma);
        *mr = _mm256_mul_ps(*mr, *mb);
        *mr = _mm256_mul_ps(*mr, *mb);
    }
}

void multiply5_loop(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat*width; i+=width)
    {
        for (size_t j=i; j<i+width; ++j)
        {
            r[j] = a[j] * a[j];
            r[j] *= a[j];
            r[j] *= b[j];
            r[j] *= b[j];
            r[j] *= b[j];
        }
    }
}

void multiply5_simd(float* a, float* b, float* r)
{
    for (size_t i=0; i<repeat; ++i)
    {
        __m256 * ma = (__m256 *) (&a[i*width]);
        __m256 * mb = (__m256 *) (&b[i*width]);
        __m256 * mr = (__m256 *) (&r[i*width]);
        *mr = _mm256_mul_ps(*ma, *ma);
        *mr = _mm256_mul_ps(*mr, *ma);
        *mr = _mm256_mul_ps(*mr, *mb);
        *mr = _mm256_mul_ps(*mr, *mb);
        *mr = _mm256_mul_ps(*mr, *mb);
    }
}

double run(std::function<void(float*,float*,float*)> func, float * arr, float * brr, float * rrr)
{
    StopWatch sw;
    std::vector<double> ecv;

    for (size_t ecount=0; ecount<necount; ++ecount)
    {
        for (size_t i=0; i<nelem; ++i)
        {
            arr[i] = i+1;
            brr[i] = i+1;
        }
        sw.lap();
        func(arr, brr, rrr);
        ecv.push_back(sw.lap());
    }

    return *std::min_element(ecv.begin(), ecv.end());
}

void check(float * rrr1, float * rrr2)
{
    size_t mismatch_count = 0;
    size_t wrong_count = 0;
    for (size_t i=0; i<nelem; ++i)
    {
        if (rrr1[i] != rrr2[i])
        {
            ++mismatch_count;
            if (std::abs((rrr1[i] - rrr2[i])/rrr1[i]) > error_tolerance)
            {
                ++wrong_count;
            }
        }
    }
    if (mismatch_count)
    {
        std::cout << "Mismatch: " << mismatch_count;
        std::cout
            << ", wrong (relative error > " << error_tolerance << "): "
            << wrong_count;
        std::cout << " / " << nelem << std::endl;
    }
}

int main(int argc, char ** argv)
{
    float * arr = (float *) aligned_alloc(32, nelem * sizeof(float));
    float * brr = (float *) aligned_alloc(32, nelem * sizeof(float));
    float * rrr1 = (float *) aligned_alloc(32, nelem * sizeof(float));
    float * rrr2 = (float *) aligned_alloc(32, nelem * sizeof(float));
    double elapsed;

    std::cout << "width: " << width << std::endl;
    std::cout << "nelem: " << nelem << std::endl;
    std::cout << std::endl;

    std::cout << "arr: 0x" << std::hex << arr << std::endl;
    std::cout << "brr: 0x" << std::hex << brr << std::endl;
    std::cout << "rrr1: 0x" << std::hex << rrr1 << std::endl;
    std::cout << "rrr2: 0x" << std::hex << rrr2 << std::endl;
    std::cout << std::endl;
    std::cout << std::dec;

    std::cout
        << "Timing repeats for " << necount << " times and takes the minimum"
        << std::endl << std::endl;

    elapsed = run(multiply1_loop, arr, brr, rrr1);
    std::cout
        << "1 multiplication by loop takes: "
        << elapsed << " sec" << std::endl;
    elapsed = run(multiply1_simd, arr, brr, rrr2);
    std::cout
        << "1 multiplication by simd takes: "
        << elapsed << " sec" << std::endl;
    check(rrr1, rrr2);
    std::cout << std::endl;

    elapsed = run(multiply3_loop, arr, brr, rrr1);
    std::cout
        << "3 multiplication by loop takes: "
        << elapsed << " sec" << std::endl;
    elapsed = run(multiply3_simd, arr, brr, rrr2);
    std::cout
        << "3 multiplication by simd takes: "
        << elapsed << " sec" << std::endl;
    check(rrr1, rrr2);
    std::cout << std::endl;

    elapsed = run(multiply5_loop, arr, brr, rrr1);
    std::cout
        << "5 multiplication by loop takes: "
        << elapsed << " sec" << std::endl;
    elapsed = run(multiply5_simd, arr, brr, rrr2);
    std::cout
        << "5 multiplication by simd takes: "
        << elapsed << " sec" << std::endl;
    check(rrr1, rrr2);
    std::cout << std::endl;

    free(arr);
    free(brr);
    free(rrr1);
    free(rrr2);
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
