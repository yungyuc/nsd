#include "fma.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char ** argv)
{
    float * arr = new float[nelem];
    float * brr = new float[nelem];
    float * crr = new float[nelem];
    float * drr1 = new float[nelem];
    float * drr2 = new float[nelem];
    double elapsed;

    StopWatch sw;

    std::cout << "width: " << width << std::endl;
    std::cout << "nelem: " << nelem << std::endl;
    std::cout << "4k: 0x" << std::hex << 4096 << std::endl;
    std::cout << "256/8: 0x" << std::hex << 256/8 << std::endl;
    std::cout << std::endl;

    std::cout << "arr: 0x" << std::hex << arr << std::endl;
    std::cout << "brr: 0x" << std::hex << brr << std::endl;
    std::cout << "crr: 0x" << std::hex << crr << std::endl;
    std::cout << "drr1: 0x" << std::hex << drr1 << std::endl;
    std::cout << "drr2: 0x" << std::hex << drr2 << std::endl;
    std::cout << std::endl;
    std::cout << std::dec;

    std::vector<double> ecv;

    std::cout
        << "Repeat for " << necount << " times for the minimal duration"
        << std::endl;

    for (size_t ecount=0; ecount<necount; ++ecount)
    {
        for (size_t i=0; i<nelem; ++i)
        {
            arr[i] = i;
            brr[i] = i;
            crr[i] = i;
        }
        sw.lap();
        multiply_and_add_loop(arr, brr, crr, drr1);
        ecv.push_back(sw.lap());
    }
    std::cout
        << "Loop takes: " << *std::min_element(ecv.begin(), ecv.end())
        << " sec" << std::endl;
    ecv.clear();

    for (size_t ecount=0; ecount<necount; ++ecount)
    {
        for (size_t i=0; i<nelem; ++i)
        {
            arr[i] = i;
            brr[i] = i;
            crr[i] = i;
        }
        sw.lap();
        multiply_and_add_simd(arr, brr, crr, drr2);
        ecv.push_back(sw.lap());
    }
    std::cout << "Simd takes: "
        << *std::min_element(ecv.begin(), ecv.end())
        << " sec" << std::endl;

    size_t mismatch_count = 0;
    size_t wrong_count = 0;
    for (size_t i=0; i<nelem; ++i)
    {
        if (drr1[i] != drr2[i])
        {
            ++mismatch_count;
            if (std::abs((drr1[i] - drr2[i])/drr1[i]) > error_tolerance)
            {
                ++wrong_count;
            }
        }
    }
    std::cout
        << "Mismatch: " << mismatch_count << ", "
        << "wrong (relative error > " << error_tolerance << "): " << wrong_count
        << " / " << nelem << std::endl;

    delete[] arr;
    delete[] brr;
    delete[] crr;
    delete[] drr1;
    delete[] drr2;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
