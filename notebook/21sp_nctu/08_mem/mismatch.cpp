#include <atomic>
#include <cstdlib>
#include <iostream>

/*
 * Memory allocated by `new` should be freed by `delete`, and `new[]` by
 * `delete[]`.  Mixing the scalar and array operators will cause error.
 */

static std::atomic_size_t tot_bytes = 0;

struct Block
{
    Block()
    {
        buffer = new int64_t[32];
        tot_bytes += 32*sizeof(int64_t);
        std::cout << "Block (" << this << ") constructed" << std::endl;
    }
    ~Block()
    {
        delete[] buffer;
        tot_bytes -= 32*sizeof(int64_t);
        std::cout << "Block (" << this << ") destructed" << std::endl;
    }
    void fill()
    {
        for (size_t it=0; it<32; ++it)
        {
            buffer[it] = static_cast<int64_t>(it);
        }
    }
    int64_t * buffer;
};

Block * allocate()
{
    Block * ptr = new Block[10];
    std::cout << "ptr: " << ptr << " allocated" << std::endl;
    return ptr;
}

void deallocate(Block * ptr)
{
    delete ptr;
}

int main(int argc, char ** argv)
{
    Block * blocks = allocate();

    for (size_t it=0; it<10; ++it)
    {
        blocks[it].fill();
    }

    deallocate(blocks);

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
