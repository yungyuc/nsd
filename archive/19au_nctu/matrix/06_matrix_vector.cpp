#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
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

    // TODO: move constructors and assignment operators.

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
 * Naive matrix vector multiplication.
 */
std::vector<double> operator*(Matrix const & mat, std::vector<double> const & vec)
{
    if (mat.ncol() != vec.size())
    {
        throw std::out_of_range("matrix column differs from vector size");
    }

    std::vector<double> ret(mat.nrow());

    for (size_t i=0; i<mat.nrow(); ++i)
    {
        double v = 0;
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            v += mat(i,j) * vec[j];
        }
        ret[i] = v;
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
    size_t width = 5;

    std::cout << ">>> square matrix-vector multiplication:" << std::endl;
    Matrix mat(width, width);

    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            mat(i, j) = i == j ? 1 : 0;
        }
    }

    std::vector<double> vec{1, 0, 0, 0, 0};
    std::vector<double> res = mat * vec;

    std::cout << "matrix A:" << mat << std::endl;
    std::cout << "vector b:" << vec << std::endl;
    std::cout << "A*b =" << res << std::endl;

    std::cout << ">>> m*n matrix-vector multiplication:" << std::endl;
    Matrix mat2(2, 3);

    double v = 1;
    for (size_t i=0; i<mat2.nrow(); ++i)
    {
        for (size_t j=0; j<mat2.ncol(); ++j)
        {
            mat2(i, j) = v;
            v += 1;
        }
    }

    std::vector<double> vec2{1, 2, 3};
    std::vector<double> res2 = mat2 * vec2;

    std::cout << "matrix A:" << mat2 << std::endl;
    std::cout << "vector b:" << vec2 << std::endl;
    std::cout << "A*b =" << res2 << std::endl;

    std::cout << ">>> transposed matrix-vector multiplication:" << std::endl;
    mat2.transpose();
    std::vector<double> vec3{1, 2};
    std::vector<double> res3 = mat2 * vec3;

    std::cout << "matrix A:" << mat2 << std::endl;
    std::cout << "matrix A buffer:" << mat2.buffer_vector() << std::endl;
    std::cout << "vector b:" << vec3 << std::endl;
    std::cout << "A*b =" << res3 << std::endl;

    std::cout << ">>> copied transposed matrix-vector multiplication:" << std::endl;
    Matrix mat3 = mat2;
    res3 = mat2 * vec3;

    std::cout << "matrix A:" << mat3 << std::endl;
    std::cout << "matrix A buffer:" << mat3.buffer_vector() << std::endl;
    std::cout << "vector b:" << vec3 << std::endl;
    std::cout << "A*b =" << res3 << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
