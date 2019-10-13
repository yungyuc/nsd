#include "StopWatch.hpp"

#include <cstddef>
#include <iostream>
#include <time.h>

int main(int argc, char ** argv)
{
    constexpr const size_t nelem = 1024 * 1024 * 64;
    // Assume row-major 2D array.
    size_t ncol = 1;
    size_t nrow = nelem / ncol;

    double * buffer = new double[nelem];
    StopWatch sw;
    double elapsed;

    for (size_t ncol=1; ncol<=1024 * 8; ncol *= 2)
    //for (size_t ncol=1024; ncol>0; ncol /= 2)
    {

        // Prepopulation to cancel the effect of overcommit or delayed allocation.
        for (size_t i=0; i<nelem; ++i) { buffer[i] = nelem-i; }

        size_t nrow = nelem / ncol;
        std::cout << "# of elements: " << nelem << " = "
                  << nrow << " x " << ncol << std::endl;

        // Populate flatly.
        sw.lap();
        for (size_t i=0; i<nelem; ++i) { buffer[i] = i; }
        elapsed = sw.lap();
        std::cout << "populate double flatly takes: "
                  << elapsed << " sec" << std::endl;

        // Populate by last index.
        sw.lap();
        for (size_t i=0; i<nrow; ++i) // the i-th row
        {
            for (size_t j=0; j<ncol; ++j) // the j-th column
            {
                buffer[i*ncol + j] = i*ncol + j;
            }
        }
        double elapsed_last = elapsed = sw.lap();
        std::cout << "populate double along last axis takes: "
                  << elapsed << " sec" << std::endl;

        // Populate by first index.
        sw.lap();
        for (size_t j=0; j<ncol; ++j) // the j-th column
        {
            for (size_t i=0; i<nrow; ++i) // the i-th row
            {
                buffer[i*ncol + j] = i*ncol + j;
            }
        }
        double elapsed_first = elapsed = sw.lap();
        std::cout << "populate double along first axis takes: "
                  << elapsed << " sec" << std::endl;

        std::cout << "ratio: " << elapsed_first / elapsed_last
                  << std::endl << std::endl;

    }

    for (size_t i=0; i<nelem; ++i) { buffer[i] += i; }
    delete[] buffer;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
