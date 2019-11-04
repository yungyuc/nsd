#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>
#include <algorithm>
#include <iostream>

template <class T>
class InstanceCounter
{

public:

    InstanceCounter() { ++m_constructed; }
    InstanceCounter(InstanceCounter const & other) { ++m_copied; }
    ~InstanceCounter() { ++m_destructed; }

    static std::size_t active()
    {
        return m_constructed + m_copied - m_destructed;
    }
    static std::size_t constructed() { return m_constructed; }
    static std::size_t copied() { return m_copied; }
    static std::size_t destructed() { return m_destructed; }

private:

    static std::atomic_size_t m_constructed;
    static std::atomic_size_t m_copied;
    static std::atomic_size_t m_destructed;

}; /* end class InstanceCounter */

// Compiler will make sure these static variables are defined only once.
template <class T> std::atomic_size_t InstanceCounter<T>::m_constructed = 0;
template <class T> std::atomic_size_t InstanceCounter<T>::m_copied = 0;
template <class T> std::atomic_size_t InstanceCounter<T>::m_destructed = 0;

struct Data
  : public InstanceCounter<Data>
{

    std::size_t buffer[1024];

}; /* end struct Data */

struct Data2
  : public InstanceCounter<Data2>
{

    Data2() = default;
    Data2(Data2 const & other)
#if 0
    // Don't forget to call the base class copy constructor.  The implicit copy
    // constructor calls it for you.  But when you have custom copy
    // constructor, if you do not specify the base constructor, the default
    // constructor in the base class is used.
      : InstanceCounter<Data2>(other)
#endif
    {
        std::copy_n(other.buffer, 1024, buffer);
    }
    Data2 & operator=(Data2 const & other)
    {
        std::copy_n(other.buffer, 1024, buffer);
        return *this;
    }

    std::size_t buffer[1024];

}; /* end struct Data */

template <class T>
void report(std::string const & prefix)
{
    using ct = InstanceCounter<T>;

    std::cout
        << prefix
        << " instance: active = " << ct::active()
        << " constructed = " << ct::constructed()
        << " copied = " << ct::copied()
        << " destructed = " << ct::destructed()
        << std::endl;
}

int main(int argc, char ** argv)
{
    std::cout << "** Creation phase **" << std::endl;

    // Data.
    Data * data = new Data();
    report<Data> ("Data  (default construction)  ");

    Data * data_copied = new Data(*data);
    report<Data> ("Data  (copy construction)     ");

    std::vector<Data> dvec(64);
    report<Data> ("Data  (construction in vector)");

    // Data2.
    Data2 * data2 = new Data2();
    report<Data2>("Data2 (default construction)  ");

    Data2 * data2_copied = new Data2(*data2);
    report<Data2>("Data2 (copy construction)     ");

    std::vector<Data2> d2vec(64);
    report<Data2>("Data2 (construction in vector)");

    std::cout << "** Deletion phase **" << std::endl;

    // Data.
    std::vector<Data>().swap(dvec);
    report<Data>("Data ");
    delete data;
    report<Data>("Data ");
    delete data_copied;
    report<Data>("Data ");

    // Data2.
    std::vector<Data2>().swap(d2vec);
    report<Data2>("Data2");
    delete data2;
    report<Data2>("Data2");
    delete data2_copied;
    report<Data2>("Data2");

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
