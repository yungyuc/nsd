#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#ifdef NOMKL
#include <lapacke.h>
#else // NOMKL
#include <mkl_lapacke.h>
#endif // NOMKL

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol, bool column_major)
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, bool column_major, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_column_major(other.m_column_major)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_column_major(other.m_column_major)
    {
        reset_buffer(0, 0);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        m_column_major = other.m_column_major;
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double * data() const { return m_buffer; }

private:

    size_t index(size_t row, size_t col) const
    {
        if (m_column_major) { return row          + col * m_nrow; }
        else                { return row * m_ncol + col         ; }
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    bool m_column_major = false;
    double * m_buffer = nullptr;

};

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(10) << mat(i, j);
        }
    }

    ostr << std::endl << " data: ";
    for (size_t i=0; i<mat.size(); ++i)
    {
        ostr << " " << mat.data()[i];
    }

    return ostr;
}

std::ostream & operator << (std::ostream & ostr, std::vector<double> const & vec)
{
    for (size_t i=0; i<vec.size(); ++i)
    {
        std::cout << " " << vec[i];
    }

    return ostr;
}

/*
 * See references:
 * * https://software.intel.com/en-us/mkl-developer-reference-c-gels
 * * https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_dgels_row.c.htm
 */
int main(int argc, char ** argv)
{
    const size_t m = 4, n = 3;
    int status;

    std::cout << ">>> least square" << std::endl;
    // Use least-square to fit the data of (x, y) tuple:
    // (1, 17), (2, 58), (3, 165), (4, 360) to
    // the equation: a_1 x^3 + a_2 x^2 + a_3 x.
    Matrix mat(m, n, false);
    mat(0,0) = 1; mat(0,1) = 1; mat(0,2) = 1;
    mat(1,0) = 8; mat(1,1) = 4; mat(1,2) = 2;
    mat(2,0) = 27; mat(2,1) = 9; mat(2,2) = 3;
    mat(3,0) = 64; mat(3,1) = 16; mat(3,2) = 4;
    std::vector<double> y{17, 58, 165, 360};
    // The equation f(x) = 3x^3 + 7^2x + 8x can perfectly fit the following
    // RHS:
    // std::vector<double> y{18, 68, 168, 336};

    std::cout << "J:" << mat << std::endl;
    std::cout << "y:" << y << std::endl;

    status = LAPACKE_dgels(
        LAPACK_ROW_MAJOR // int matrix_layout
      , 'N' // transpose; 'N' is no transpose, 'T' is transpose, 'C' conjugate transpose
      , m // number of rows of matrix
      , n // number of columns of matrix
      , 1 // nrhs; number of columns of RHS
      , mat.data() // a; the 'J' matrix
      , n // lda; leading dimension of matrix
      , y.data() // b; RHS
      , 1 // ldb; leading dimension of RHS
    );

    std::cout << "dgels status: " << status << std::endl;
    std::cout << "a: " << y << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
