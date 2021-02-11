#include <iostream>
#include <iomanip>
#include <cstdlib>

struct PlainData
{
    int buffer[1024*8];
}; /* end struct PlainData */

std::ostream & put_ptr(std::ostream & out, void * ptr)
{
    out << std::internal << std::setw(18) << std::setfill('0') << ptr;
    return out;
}

// The factory function for PlainData.
PlainData * make_data()
{
    PlainData * ptr = new PlainData();
    // (... work to be done before returning.)
    return ptr;
}

void manipulate_with_reference(PlainData & data)
{
    std::cout << "Manipulate with reference      : ";
    put_ptr(std::cout, &data) << std::endl;

    for (size_t it=0; it < 1024*8; ++it)
    {
        data.buffer[it] = it;
    }
    // (... more meaningful work before returning.)

    // We cannot delete an object passed in with a reference.
}

int main(int, char **)
{
    PlainData * ptr = nullptr;

    // Obtain the pointer to the object ('resource').
    ptr = make_data();
    std::cout << "PlainData pointer after factory: ";
    put_ptr(std::cout, ptr) << std::endl;

    manipulate_with_reference(*ptr);

    // A good habit when using raw pointer: destruct the object in the scope
    // that we obtain the pointer.  In this way, we don't forget to delete it
    // and avoid potential resource leak.
    delete ptr;
    std::cout << "PlainData pointer after delete : ";
    put_ptr(std::cout, ptr) << std::endl;

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
