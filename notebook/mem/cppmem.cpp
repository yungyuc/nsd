#include <cstdlib>
#include <iostream>

/*
 * A dummy class taking 8k bytes.
 */
struct Block
{
    Block()
    {
        std::cout << "Block (" << this << ") constructed" << std::endl;
    }
    ~Block()
    {
        std::cout << "Block (" << this << ") destructed" << std::endl;
    }
    int64_t buffer[1024];
};

void scalar_form()
{
    std::cout
        << "frame address of scalar_form: " << __builtin_frame_address(0)
        << std::endl;

    // Doing this place 8k bytes on stack.
    Block block_stack;
    for (size_t it = 0; it < 1024; ++it)
    {
        block_stack.buffer[it] = 1000 + it;
    }
    std::cout << "object on stack: " << &block_stack << std::endl;
    std::cout
        << "address difference: "
        << reinterpret_cast<std::size_t>(__builtin_frame_address(0))
         - reinterpret_cast<std::size_t>(&block_stack)
        << ", sizeof(Block): " << sizeof(Block)
        << std::endl;

    // Use the new expression.  Note that this "new" is an expression.  It
    // calls the operator ("::operator new"), but not the operator itself.
    Block * block_dynamic = new Block;
    std::cout << "object on dynamic memory: " << block_dynamic << std::endl;

    for (size_t it = 0; it < 1024; ++it)
    {
        block_dynamic->buffer[it] = 2000 + it;
    }
    std::cout << "=== new tested" << std::endl;

    // The delete expression that destruct and deallocate the memory of the
    // dynamic block object.  Similarly, the expression calls ::operator delete
    // for block_dynamic.
    delete block_dynamic;
    std::cout << "=== delete tested" << std::endl;
}

void array_form()
{
    // An array on the stack.  It is popped away when execution leaves this
    // function.  You cannot use the memory outside this function.
    int64_t data_stack[32];

    for (size_t it = 0; it < 32; ++it)
    {
        data_stack[it] = 100 + it;
    }
    std::cout << "stack array memory: " << data_stack << std::endl;

    // A dynamic array.
    int64_t * data_dynamic = new int64_t[32];

    for (size_t it = 0; it < 32; ++it)
    {
        data_dynamic[it] = 200 + it;
    }
    std::cout << "dynamic array memory: " << data_dynamic << std::endl;
    std::cout << "=== new[] tested" << std::endl;

    delete[] data_dynamic;
    std::cout << "=== delete[] tested" << std::endl;
}

void placement()
{
    char * buffer = new char[sizeof(Block)];

    Block * block = new (buffer) Block;
    for (size_t it = 0; it < 1024; ++it)
    {
        block->buffer[it] = it;
    }
    std::cout << "=== placement new tested" << std::endl;

#if 0
    // This induces undefined behavior.  Don't do this.
    delete block;
#endif

    // Instead of deleting the pointer block, call explicit the destructor and
    // delete the original buffer.
    block->~Block();
    delete[] buffer;
}

int main(int argc, char ** argv)
{
    scalar_form();
    array_form();
    placement();

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
