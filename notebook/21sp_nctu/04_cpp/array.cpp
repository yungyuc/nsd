#include <cstdint>
#include <stdexcept>
#include <array>
#include <iostream>

int main(int, char **)
{
    std::cout << "sizeof(std::array<int, 3>) = " << sizeof(std::array<int, 3>) << std::endl;
    std::cout << "sizeof(std::array<int, 10>) = " << sizeof(std::array<int, 10>) << std::endl;

    std::array<int, 3> arr1;

    // Without bound check.
    for (size_t it=0; it<arr1.size(); ++it)
    { arr1[it] = 100+it; }

    // With bound check.
    for (size_t it=0; it<arr1.size(); ++it)
    { arr1.at(it) = 100+it; }

    std::cout << "arr1:" << std::endl;
    for (int const v : arr1)
    { std::cout << "  " << v << std::endl; }

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
