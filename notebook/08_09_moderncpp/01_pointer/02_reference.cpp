#include <iostream>
#include <cstdlib>

struct PlainData
{
    int buffer[1024*8];
}; /* end struct PlainData */

// The factory function for PlainData.
PlainData * make_data()
{
    PlainData * ptr = new PlainData();
    // In a real factory function, we will do something with the allocated data
    // object before returning it.
    return ptr;
}

void manipulate_with_reference(PlainData & data)
{
    std::cout << "Manipulate with reference: " << &data << std::endl;

    for (size_t it=0; it < 1024*8; ++it)
    {
        data.buffer[it] = it;
    }
    // In a real consumer function we will do much more meaningful operations.

    // However, we cannot destruct an object passed in with a reference.
}

int main(int argc, char ** argv)
{
    PlainData * ptr = nullptr;

    ptr = make_data();
    std::cout << "PlainData pointer after factory: " << ptr << std::endl;

    manipulate_with_reference(*ptr);

    // Destruct the object where we have the pointer.
    delete ptr;
    std::cout << "PlainData pointer after delete: " << ptr << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
