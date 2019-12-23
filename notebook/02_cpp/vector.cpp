#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

int main(int, char **)
{
    std::cout << "sizeof(std::vector<int>) = " << sizeof(std::vector<int>) << std::endl;
    std::cout << "sizeof(std::vector<double>) = " << sizeof(std::vector<double>) << std::endl;
    std::cout << "sizeof(std::vector<std::vector<double>>) = "
              << sizeof(std::vector<std::vector<double>>) << std::endl;

    // Populate with an indexed loop.
    std::vector<int> vec0;
    for (size_t it=0; it<100; ++it) { vec0.push_back(it+1000); }

    // Populate with iterators.
    std::vector<int> vec1(vec0.begin()+20, vec0.begin()+25);
    std::cout << "vec1 indices [20-25):" << std::endl;
    for (int const v : vec1) { std::cout << "  " << v << std::endl; }

    try
    { vec1.at(10); } // Bound check.
    catch (std::out_of_range e)
    { std::cout << "out of range exception: " << e.what() << std::endl; }

    vec1.at(2) = 500;
    vec1[3] = 600; // No bound check; dangerous.
    std::cout << "vec1 modified:" << std::endl;
    for (int const v : vec1) { std::cout << "  " << v << std::endl; }

    // The internal buffer may change from reallocation.
    std::vector<int> data(10);
    const int * ptr0 = data.data();
    std::cout << "&data.at(0) = " << ptr0 << std::endl;
    data.resize(100);
    for (size_t it=0; it<10; ++it) { data.push_back(it+100); }
    const int * ptr1 = data.data();
    std::cout << "&data.at(0) = " << ptr1 << std::endl;
    if (ptr0 != ptr1)
    {
        std::cout << "oops, address changes" << std::endl;
    }

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
