#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>

class IsCopied
{

public:

    static IsCopied & instance()
    {
        static IsCopied inst;
        return inst;
    }

    IsCopied & on() { m_status = true; return *this; }

    operator bool() const { return m_status; }

    ~IsCopied() = default;

private:

    IsCopied() : m_status(false) {}

    IsCopied(IsCopied const & ) = delete;
    IsCopied(IsCopied       &&) = delete;
    IsCopied & operator=(IsCopied const & ) = delete;
    IsCopied & operator=(IsCopied       &&) = delete;

    bool m_status;

}; /* end class IsCopied */

class Data
{

public:

    constexpr const static size_t NELEM = 1024*8;

    Data()
    {
        std::cout << "Data constructed @" << this << std::endl;
    }

    Data(Data const & other)
    {
        copy_from(other);
        std::cout << "Data copied to @" << this << " from @" << &other << std::endl;
    }

    Data & operator=(Data const & other)
    {
        copy_from(other);
        std::cout << "Data copy assigned to @" << this << " from @" << &other << std::endl;
        return *this;
    }

    ~Data()
    {
        std::cout << "Data destructed @" << this << std::endl;
    }

    size_t size() const { return NELEM; }
    int   operator[](size_t it) const { return m_buffer[it]; }
    int & operator[](size_t it)       { return m_buffer[it]; }

    bool is_manipulated() const
    {
        for (size_t it=0; it < size(); ++it)
        {
            if ((*this)[it] != it) { return false; }
        }
        return true;
    }

private:

    void copy_from(Data const & other)
    {
        for (size_t it=0; it < NELEM; ++it)
        {
            m_buffer[it] = other.m_buffer[it];
        }
        IsCopied::instance().on();
    }

    // A lot of data that we don't want to reconstruct.
    int m_buffer[NELEM];

}; /* end class Data */

void manipulate_with_reference(Data & data, int value)
{
    std::cout << "Manipulate with reference: " << &data << std::endl;

    for (size_t it=0; it < data.size(); ++it)
    {
        data[it] = value + it;
    }
    // In a real consumer function we will do much more meaningful operations.

    // However, we cannot destruct an object passed in with a reference.
}

Data worker1()
{
    Data data;

    // Manipulate the Data object.
    manipulate_with_reference(data, 3);

    return data;
}

Data worker2()
{
    Data data = worker1();

    // Manipulate the Data object, again.
    manipulate_with_reference(data, 8);

    return data;
}

int main(int argc, char ** argv)
{
    std::cout
        << (bool(IsCopied::instance()) ? "Something" : "Nothing")
        << " is copied" << std::endl;
    Data data = worker2();
    std::cout
        << (bool(IsCopied::instance()) ? "Something" : "Nothing")
        << " is copied" << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
