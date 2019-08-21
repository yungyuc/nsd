#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __APPLE__
// Macos hasn't implemented the C11 aligned_alloc as of the time 2019/8.
void * aligned_alloc(size_t alignment, size_t size)
{
    void * ptr;
    posix_memalign(&ptr, alignment, size);
    return ptr;
}
#endif

int64_t * inner();

void outer()
{
    printf("frame address of outer: %p\n", __builtin_frame_address(0));

    int64_t * data = inner();
    printf("data returned from inner: %p\n", &data);

    for (size_t it = 0; it < 32; ++it)
    {
        if (data[it] != 200 + it)
        {
            printf("error\n");
        }
    }
    printf("=== malloc tested\n");

    // You must free the memory after you finish using it.  Otherwise it will
    // remain in the process unclaimed, results in the "memory leak".
    free(data);
    printf("=== free tested\n");

    /* You may not use the memory that is already freed.  The results is
     * undefined.
    for (size_t it = 0; it < 32; ++it)
    {
        if (data[it] != 200 + it)
        {
            printf("error\n");
        }
    }
    */

    // The following two allocations result in the same zero-initialized array.
    // But when the OS has already zero initialized the memory, calloc may run
    // faster than malloc-then-fill.
    data = (int64_t *) calloc(32, sizeof(int64_t));
    free(data);
    data = (int64_t *) malloc(32 * sizeof(int64_t));
    for (size_t it = 0; it < 32; ++it) { data[it] = 0; }
    free(data);
    printf("=== calloc tested\n");

    // Reallocate the memory with smaller or larger size.
    data = (int64_t *) malloc((1UL << 20) * 2 * sizeof(int64_t));
    printf("address by malloc: %p\n", data);
    data = (int64_t *) realloc(data, (1UL << 20) * 1 * sizeof(int64_t));
    printf("address by realloc to smaller memory: %p\n", data);
    data = (int64_t *) realloc(data, (1UL << 20) * 4 * sizeof(int64_t));
    printf("address by realloc to larger memory: %p\n", data);
    free(data);
    printf("=== realloc tested\n");

    // Aligned allocation.
    int64_t * data1 = (int64_t *) malloc(sizeof(int64_t));
    printf("address by malloc: %p\n", data1);
    int64_t * data2 = (int64_t *) aligned_alloc(256, 256 * sizeof(int64_t));
    printf("address by aligned_alloc: %p\n", data2);
    free(data1);
    free(data2);
    printf("=== aligned_alloc tested\n");
}

int64_t * inner()
{
    printf("frame address of inner: %p\n", __builtin_frame_address(0));

    // An array on the stack.  It is popped away when execution leaves this
    // function.  You cannot use the memory outside this function.
    int64_t data_stack[32];

    for (size_t it = 0; it < 32; ++it)
    {
        data_stack[it] = 100 + it;
    }
    printf("stack memory: %p\n", data_stack);

    // A dynamic array.
    int64_t * data_dynamic = (int64_t *) malloc(32 * sizeof(int64_t));

    for (size_t it = 0; it < 32; ++it)
    {
        data_dynamic[it] = 200 + it;
    }
    printf("dynamic memory: %p\n", data_dynamic);

    return data_dynamic;
}

int main(int argc, char ** argv)
{
    printf("frame address of main: %p\n", __builtin_frame_address(0));

    outer();

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
