#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <iterator>
#include <vector>

class Data
{

public:

    constexpr const static size_t NELEM = 1024*8;

    Data(size_t serial)
      : m_serial(serial)
    {
        m_buffer = new int[NELEM];
        initialize();
        std::cout << "Data #" << m_serial << " constructed @" << this << std::endl;
    }

    Data(Data const & other)
    {
        m_serial = other.m_serial;
        m_buffer = new int[NELEM];
        copy_from(other);
        std::cout << "Data #" << m_serial << " copied to @" << this << " from @" << &other << std::endl;
    }

    Data & operator=(Data const & other)
    {
        m_serial = other.m_serial;
        if (nullptr == m_buffer) { m_buffer = new int[NELEM]; }
        copy_from(other);
        std::cout << "Data #" << m_serial << " copy assigned to @" << this << " from @" << &other << std::endl;
        return *this;
    }

#ifdef MOVENOEXCEPT
    Data(Data && other) noexcept
#else // MOVENOEXCEPT
    Data(Data && other)
#endif // MOVENOEXCEPT
    {
        m_serial = other.m_serial;
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        std::cout << "Data #" << m_serial << " moved to @" << this << " from @" << &other << std::endl;
    }

    Data & operator=(Data && other)
    {
        m_serial = other.m_serial;
        if (m_buffer) { delete[] m_buffer; }
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        std::cout << "Data #" << m_serial << " move assigned to @" << this << " from @" << &other << std::endl;
        return *this;
    }

    ~Data()
    {
        if (m_buffer) { delete[] m_buffer; }
        std::cout << "Data #" << m_serial << " destructed @" << this << std::endl;
    }

    size_t size() const { return NELEM; }
    int   operator[](size_t it) const { return m_buffer[it]; }
    int & operator[](size_t it)       { return m_buffer[it]; }

    bool is_initialized() const
    {
        for (size_t it=0; it < size(); ++it)
        {
            if ((*this)[it] != it) { return false; }
        }
        return true;
    }

private:

    void initialize()
    {
        for (size_t it=0; it < size(); ++it)
        {
            (*this)[it] = it;
        }
    }

    void copy_from(Data const & other)
    {
        for (size_t it=0; it < NELEM; ++it)
        {
            m_buffer[it] = other.m_buffer[it];
        }
    }

    size_t m_serial;

    // A lot of data that we don't want to reconstruct.
    int * m_buffer;

}; /* end class Data */

std::vector<Data> inner1(size_t start, size_t len)
{
    std::cout << "** inner1 begins with " << start << std::endl;
    std::vector<Data> ret;
    for (size_t it=0; it < len; ++it)
    {
        Data data(start+it);
        ret.emplace_back(std::move(data));
    }
    return ret;
}

void outer1(size_t len)
{
    std::cout << "* outer1 begins" << std::endl;
    std::vector<Data> vec;
    for (size_t it=0; it < len; ++it)
    {
        std::cout << std::endl;
        std::cout << "* outer1 loop it=" << it << " begins" << std::endl;
        std::vector<Data> subvec = inner1(vec.size(), it+1);
        std::cout << "* outer1 obtained inner1 at " << vec.size() << std::endl;
        vec.insert(
            vec.end()
          , std::make_move_iterator(subvec.begin())
          , std::make_move_iterator(subvec.end())
        );
        std::cout << "* outer1 inserted subvec.size()=" << subvec.size() << std::endl;
    }
    std::cout << "* outer1 result.size() = " << vec.size() << std::endl << std::endl;
}

void inner2(size_t start, size_t len, std::vector<Data> & result /* for output */)
{
    std::cout << "** inner2 begins with " << start << std::endl;
    for (size_t it=0; it < len; ++it)
    {
        Data data(start+it);
        result.emplace_back(std::move(data));
    }
}

void outer2(size_t len)
{
    std::cout << "* outer2 begins" << std::endl;
    std::vector<Data> vec;
    for (size_t it=0; it < len; ++it)
    {
        std::cout << std::endl;
        std::cout << "* outer2 loop it=" << it << " begins" << std::endl;
        inner2(vec.size(), it+1, vec);
    }
    std::cout << "* outer2 result.size() = " << vec.size() << std::endl << std::endl;
}

struct Accumulator
{

public:
    // This can be called if consumers want the sub-operation one by one, and
    // make the code more testable. But it isn't really used in the example.
    std::vector<Data> inner1(size_t start, size_t len)
    {
        std::cout << "** Accumulator::inner1 begins with " << start << std::endl;
        std::vector<Data> ret;
        ret.reserve(len);
        inner2(start, len, ret);
        return ret;
    }

private:
    void inner2(size_t start, size_t len, std::vector<Data> & ret)
    {
        std::cout << "** Accumulator::inner2 begins with " << start << std::endl;
        for (size_t it=0; it < len; ++it)
        {
            Data data(start+it);
            ret.emplace_back(std::move(data));
        }
    }

public:
    // This is used when batch operation is in demand.
    void outer(size_t len)
    {
        std::cout << "* Accumulator::outer begins" << std::endl;
        result.reserve(len*(len+1)/2);
        for (size_t it=0; it < len; ++it)
        {
            std::cout << std::endl;
            std::cout << "* Accumulator::outer loop it=" << it << " begins" << std::endl;
            inner2(result.size(), it+1, result);
        }
        std::cout << "* Accumulator::outer result.size() = " << result.size() << std::endl << std::endl;
    }

public:
    std::vector<Data> result;

}; /* end struct Accumulator */

int main(int argc, char ** argv)
{
#ifndef OTYPE
#define OTYPE 1
#endif

#if OTYPE == 1
    outer1(3);
#elif OTYPE == 2
    outer2(3);
#elif OTYPE == 3
    Accumulator().outer(3);
#endif
}

// vim: set et sw=4 ts=4 sts=4:
