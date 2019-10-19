#include <iostream>
#include <iomanip>

int main(int argc, char ** argv)
{
    constexpr size_t width = 5;

    double vector[width];

    // Populate a vector.
    for (size_t i=0; i<width; ++i)
    {
        vector[i] = i;
    }

    std::cout << "vector elements in memory:" << std::endl << " ";
    for (size_t i=0; i<width; ++i)
    {
        std::cout << " " << vector[i];
    }
    std::cout << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
