#include "StopWatch.hpp"

#include <cstddef>
#include <iostream>

int main(int argc, char ** argv)
{
    constexpr const size_t nelem = 128 * 1024 * 1024;
    int * arr = new int[nelem];
    double elapsed;

    StopWatch sw;

    // Sequential.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; ++i) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Sequential takes: " << elapsed << " sec" << std::endl;

    std::cout << std::endl;

    // Skipping 2.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=2) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 2 takes: " << elapsed << " sec" << std::endl;

    // Skipping 4.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=4) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 4 takes: " << elapsed << " sec" << std::endl;

    // Skipping 8.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=8) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 8 takes: " << elapsed << " sec" << std::endl;

    // Skipping 16.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=16) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 16 takes: " << elapsed << " sec" << std::endl;

    std::cout << std::endl;

    // Skipping 32.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=32) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 32 takes: " << elapsed << " sec" << std::endl;

    // Skipping 64.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=64) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 64 takes: " << elapsed << " sec" << std::endl;

    // Skipping 128.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=128) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 128 takes: " << elapsed << " sec" << std::endl;

    // Skipping 256.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=256) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 256 takes: " << elapsed << " sec" << std::endl;

    // Skipping 512.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=512) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 512 takes: " << elapsed << " sec" << std::endl;

    // Skipping 1024.
    for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
    sw.lap();
    for (size_t i=0; i<nelem; i+=1024) { arr[i] *= 3; }
    elapsed = sw.lap();
    std::cout << "Skipping 1024 takes: " << elapsed << " sec" << std::endl;

    delete[] arr;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
