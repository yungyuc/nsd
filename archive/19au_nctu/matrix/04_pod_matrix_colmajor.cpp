#include <iostream>
#include <iomanip>

int main(int argc, char ** argv)
{
    constexpr size_t width = 5;

    double * buffer = new double[width*width];
    double (*matrix)[width] = reinterpret_cast<double (*)[width]>(buffer);
    std::cout << "buffer address: " << buffer << std::endl
              << "matrix address: " << matrix << std::endl;

    // Populate a buffer (column-major 2D array).
    for (size_t i=0; i<width; ++i) // the i-th row
    {
        for (size_t j=0; j<width; ++j) // the j-th column
        {
            buffer[j*width + i] = i*10 + j;
        }
    }

    std::cout << "matrix (column-major) elements as 2D array:";
    for (size_t i=0; i<width; ++i)
    {
        std::cout << std::endl << " ";
        for (size_t j=0; j<width; ++j)
        {
            std::cout << " " << std::setfill('0') << std::setw(2)
                      << matrix[j][i];
        }
    }
    std::cout << std::endl;

    std::cout << "matrix (column-major) elements in memory:" << std::endl << " ";
    for (size_t i=0; i<width*width; ++i)
    {
        std::cout << " " << std::setfill('0') << std::setw(2) << buffer[i];
    }
    std::cout << std::endl;
    std::cout << "column majoring: "
              << "the fastest moving index is the leading index"
              << std::endl;

    delete[] buffer;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
