#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>

class Status
{

public:

    static Status & instance()
    {
        static Status inst;
        return inst;
    }

    bool is_copied() const { return m_copied; }
    bool is_moved() const { return m_moved; }

    void set_copied() { m_copied = true; }
    void set_moved() { m_moved = true; }

private:

    Status() : m_copied(false), m_moved(false) {}

    bool m_copied;
    bool m_moved;

}; /* end class Status */

class Data
{

public:

    constexpr const static size_t NELEM = 1024*8;

    Data()
    {
        m_buffer = new int[NELEM];
        std::cout << "Data constructed @" << this << std::endl;
    }

    Data(Data const & other)
    {
        m_buffer = new int[NELEM];
        copy_from(other);
        std::cout << "Data copied to @" << this << " from @" << &other << std::endl;
    }

    Data & operator=(Data const & other)
    {
        if (nullptr == m_buffer) { m_buffer = new int[NELEM]; }
        copy_from(other);
        std::cout << "Data copy assigned to @" << this << " from @" << &other << std::endl;
        return *this;
    }

    Data(Data && other)
    {
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        std::cout << "Data moved to @" << this << " from @" << &other << std::endl;
        Status::instance().set_moved();
    }

    Data & operator=(Data && other)
    {
        if (m_buffer) { delete[] m_buffer; }
        m_buffer = other.m_buffer;
        other.m_buffer = nullptr;
        std::cout << "Data move assigned to @" << this << " from @" << &other << std::endl;
        Status::instance().set_moved();
        return *this;
    }

    ~Data()
    {
        if (m_buffer) { delete[] m_buffer; }
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

    void reset()
    {
    }

    void copy_from(Data const & other)
    {
        for (size_t it=0; it < NELEM; ++it)
        {
            m_buffer[it] = other.m_buffer[it];
        }
        Status::instance().set_copied();
    }

    // A lot of data that we don't want to reconstruct.
    int * m_buffer;

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

#ifdef FORCEMOVE
    // Explicit move semantics destroys copy elision.
    return std::move(data);
#else
    return data;
#endif
}

int main(int argc, char ** argv)
{
    std::cout
        << "Status:"
        << (bool(Status::instance().is_copied()) ? " copied" : " uncopied")
        << (bool(Status::instance().is_moved()) ? " moved" : " unmoved")
        << std::endl;
    Data data = worker2();
    std::cout
        << "Status:"
        << (bool(Status::instance().is_copied()) ? " copied" : " uncopied")
        << (bool(Status::instance().is_moved()) ? " moved" : " unmoved")
        << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
