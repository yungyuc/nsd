#include <iostream>
#include <iomanip>

void work(double * buffer, size_t width)
{
    // This won't work since width isn't known in compile time.
    double (*matrix)[width] = reinterpret_cast<double (*)[width]>(buffer);

    for (size_t i=0; i<width; ++i) // the i-th row
    {
        for (size_t j=0; j<width; ++j) // the j-th column
        {
            matrix[i][j] = i*10 + j;
        }
    }

    std::cout << "matrix:";
    for (size_t i=0; i<width; ++i)
    {
        std::cout << std::endl << " ";
        for (size_t j=0; j<width; ++j)
        {
            std::cout << " " << std::setfill('0') << std::setw(2)
                      << matrix[i][j];
        }
    }
    std::cout << std::endl;
}

int main(int argc, char ** argv)
{
    size_t width = 5;

    double * buffer = new double[width*width];

    work(buffer, width);

    delete[] buffer;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
