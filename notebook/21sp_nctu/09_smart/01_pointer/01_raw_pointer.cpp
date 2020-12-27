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

int main(int, char **)
{
    std::cout << "PlainData pointer initialized : ";
    // It is a good practice to initialize a raw pointer to nullptr.
    PlainData * ptr = nullptr;
    // Although nullptr will be integer 0, do not use the integer literal 0 or
    // the infamous macro NULL to represent nullity.
    put_ptr(std::cout, ptr) << std::endl;

    // The reason to not use 0 or NULL for the null pointer: they are not even
    // of a pointer type!
    static_assert(!std::is_pointer<decltype(0)>::value, "error");
    static_assert(!std::is_pointer<decltype(NULL)>::value, "error");
    // 0 is int
    static_assert(std::is_same<decltype(0), int>::value, "error");
    // int cannot be converted to a pointer.
    static_assert(!std::is_convertible<decltype(0), void *>::value, "error");
    // NULL is long
    static_assert(std::is_same<decltype(NULL), long>::value, "error");
    // long cannot be converted to a pointer, either.
    static_assert(!std::is_convertible<decltype(NULL), void *>::value, "error");

    // Although nullptr is of type std::nullptr_t, not exactly a pointer ...
    static_assert(std::is_same<decltype(nullptr), std::nullptr_t>::value, "error");
    static_assert(!std::is_pointer<decltype(nullptr)>::value, "error");
    // It can be converted to a pointer.
    static_assert(std::is_convertible<decltype(nullptr), void *>::value, "error");
    static_assert(std::is_convertible<decltype(nullptr), PlainData *>::value, "error");

    // Allocate memory for PlainData and get the returned pointer.
    std::cout << "PlainData pointer after malloc: ";
    ptr = static_cast<PlainData *>(malloc(sizeof(PlainData)));
    put_ptr(std::cout, ptr) << std::endl;

    // After free the memory, the pointer auto variable is not changed.
    std::cout << "PlainData pointer after free  : ";
    free(ptr);
    put_ptr(std::cout, ptr) << std::endl;

    // Use new to allocate for and construct PlainData and get the returned
    // pointer.
    std::cout << "PlainData pointer after new   : ";
    ptr = new PlainData();
    put_ptr(std::cout, ptr) << std::endl;

    // After delete, the pointer auto variable is not changed, either.
    std::cout << "PlainData pointer after delete: ";
    delete ptr;
    put_ptr(std::cout, ptr) << std::endl;

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
