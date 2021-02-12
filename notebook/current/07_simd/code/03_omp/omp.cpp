#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int, char**) {
    #pragma omp parallel
    {
        printf
        (
            "Hello from thread %d, nthreads %d\n"
          , omp_get_thread_num()
          , omp_get_num_threads()
        );
    }
    return EXIT_SUCCESS;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
