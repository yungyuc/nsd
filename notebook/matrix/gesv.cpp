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

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
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
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
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
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
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
        m_transpose = other.m_transpose;
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

    bool is_transposed() const { return m_transpose; }

    Matrix & transpose()
    {
        m_transpose = !m_transpose;
        std::swap(m_nrow, m_ncol);
        return *this;
    }

private:

    size_t index(size_t row, size_t col) const
    {
        if (m_transpose) { return row          + col * m_nrow; }
        else             { return row * m_ncol + col         ; }
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
    bool m_transpose = false;
    double * m_buffer = nullptr;

};

/*
 * Naive matrix matrix multiplication.
 */
Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
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

int main(int argc, char ** argv)
{
    const size_t n = 3;
    int status;

    std::cout << ">>> Solve Ax=b (row major)" << std::endl;
    Matrix mat(n, n, std::vector<double>{3, 5, 2, 2, 1, 3, 4, 3, 2});
    Matrix b = Matrix(2, n, std::vector<double>{57, 22, 41, 23, 12, 84}).transpose();
    std::vector<int> ipiv(n);

    std::cout << "A:" << mat << std::endl;
    std::cout << "b:" << b << std::endl;

    status = LAPACKE_dgesv(
        LAPACK_ROW_MAJOR // matrix_layout
      , n // n
      , b.ncol() // nrhs
      , mat.data() // a
      , mat.ncol() // lda
      , ipiv.data() // ipiv
      , b.data() // b
      , b.ncol() // ldb
      // for row major matrix, ldb becomes the trailing dimension.
    );

    std::cout << "solution x:" << b << std::endl;
    std::cout << "dgesv status: " << status << std::endl;

    std::cout << ">>> Solve Ax=b (column major)" << std::endl;
    Matrix mat2 = Matrix(n, n, std::vector<double>{3, 5, 2, 2, 1, 3, 4, 3, 2}).transpose();
    Matrix b2(2, n, std::vector<double>{57, 22, 41, 23, 12, 84});

    std::cout << "A^t:" << mat2 << std::endl;
    std::cout << "b^t:" << b2 << std::endl;

    status = LAPACKE_dgesv(
        LAPACK_COL_MAJOR // matrix_layout
      , n // n
      , b2.nrow() // nrhs
      , mat2.data() // a
      , mat2.nrow() // lda
      , ipiv.data() // ipiv
      , b2.data() // b
      , b2.ncol() // ldb
      // for column major matrix, ldb remains the leading dimension.
    );

    std::cout << "solution x^t:" << b2 << std::endl;
    std::cout << "dgesv status: " << status << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
