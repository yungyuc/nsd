#include <type_traits>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <iterator>
#include <vector>

class Data
  : std::enable_shared_from_this<Data>
{

private:

    class ctor_passkey {};

public:

    constexpr const static size_t NELEM = 1024*8;

    /* 'create' only expose some of the constructors */
    static std::shared_ptr<Data> create(size_t serial)
    {
        return std::make_shared<Data>(serial, ctor_passkey());
    }

    /* 'make' unconditionally forward to every constructor */
    template < typename ... Args >
    static std::shared_ptr<Data> make(Args && ... args)
    {
        return std::make_shared<Data>(std::forward<Args>(args) ..., ctor_passkey());
    }

    Data(size_t serial, ctor_passkey const &)
      : m_serial(serial)
    {
        m_buffer = new int[NELEM];
        initialize(0);
        std::cout << "Data #" << m_serial << " constructed @" << this
                  << "(serial=" << m_serial << ")" << std::endl;
    }

    Data(size_t serial, int base, ctor_passkey const &)
      : m_serial(serial+base)
    {
        m_buffer = new int[NELEM];
        initialize(0);
        std::cout << "Data #" << m_serial << " constructed @" << this
                  << "(serial=" << m_serial << ")"
                  << "(base=" << base << ")" << std::endl;
    }

    // Proxy to copy and move constructor.
    Data(Data const &  other, ctor_passkey const &) : Data(std::forward<Data const &>(other)) {}
    Data(Data       && other, ctor_passkey const &) : Data(std::forward<Data &&>(other)) {}

    Data(Data const & other)
    {
        m_serial = other.m_serial;
        m_buffer = new int[NELEM];
        copy_from(other);
        std::cout << "Data #" << m_serial << " copied to @" << this << " from @" << &other << std::endl;
    }

    Data(Data && other) noexcept
    {
        m_serial = other.m_serial;
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        std::cout << "Data #" << m_serial << " moved to @" << this << " from @" << &other << std::endl;
    }

    // Turn off default constructor.
    Data() = delete;

    // Turn off assignment operators before we know how they should behave.
    Data & operator=(Data const & ) = delete;
    Data & operator=(Data       &&) = delete;

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

    void initialize(int base)
    {
        for (size_t it=0; it < size(); ++it)
        {
            (*this)[it] = base + it;
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

std::vector<std::shared_ptr<Data>> inner1(size_t base, size_t len)
{
    std::cout << "** inner1 begins with " << base << std::endl;
    std::vector<std::shared_ptr<Data>> ret;
    for (size_t it=0; it < len; ++it)
    {
        std::shared_ptr<Data> data;
        if (0 == base)
        {
#ifdef USE_CREATE
            data = Data::create(it);
#else
            data = Data::make(it);
#endif
        }
        else
        {
#ifdef USE_CREATE
            data = Data::create(it, base);
#else
            data = Data::make(it, base);
#endif
        }
        ret.emplace_back(data);
    }
    return ret;
}

void outer1(size_t len)
{
    std::cout << "* outer1 begins" << std::endl;
    std::vector<std::shared_ptr<Data>> vec;
    for (size_t it=0; it < len; ++it)
    {
        std::cout << std::endl;
        std::cout << "* outer1 loop it=" << it << " begins" << std::endl;
        std::vector<std::shared_ptr<Data>> subvec = inner1(vec.size(), it+1);
        std::cout << "* outer1 obtained inner1 at " << vec.size() << std::endl;
        vec.insert(
            vec.end()
          , std::make_move_iterator(subvec.begin())
          , std::make_move_iterator(subvec.end())
        );
        std::cout << "* outer1 inserted subvec.size()=" << subvec.size() << std::endl;
    }
    std::cout << "* outer1 result.size() = " << vec.size() << std::endl << std::endl;

#ifdef SHOW_PERFECT_FORWARD
    vec.emplace_back(Data::make(*vec[0]));
    vec.emplace_back(Data::make(std::move(*vec[1])));
#endif

    std::cout << "* outer1 end" << std::endl << std::endl;
}

int main(int argc, char ** argv)
{
    outer1(3);
}

// vim: set et sw=4 ts=4 sts=4:
