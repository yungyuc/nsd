#include <iostream>
#include <iomanip>

int main(int argc, char ** argv)
{
    constexpr size_t width = 5;

    double amatrix[width][width];

    // Populate the matrix on stack (row-major 2D array).
    for (size_t i=0; i<width; ++i) // the i-th row
    {
        for (size_t j=0; j<width; ++j) // the j-th column
        {
            amatrix[i][j] = i*10 + j;
        }
    }

    std::cout << "2D array elements:";
    for (size_t i=0; i<width; ++i)
    {
        std::cout << std::endl << " ";
        for (size_t j=0; j<width; ++j)
        {
            std::cout << " " << std::setfill('0') << std::setw(2)
                      << amatrix[i][j];
        }
    }
    std::cout << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
