#include <iostream>
#include <cstdlib>

struct PlainData
{
    int buffer[1024*8];
}; /* end struct PlainData */

int main(int argc, char ** argv)
{
    PlainData * ptr = nullptr;

    ptr = static_cast<PlainData *>(malloc(sizeof(PlainData)));
    std::cout << "PlainData pointer after malloc: " << ptr << std::endl;

    free(ptr);
    std::cout << "PlainData pointer after free: " << ptr << std::endl;

    ptr = new PlainData();
    std::cout << "PlainData pointer after new: " << ptr << std::endl;

    delete ptr;
    std::cout << "PlainData pointer after delete: " << ptr << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
