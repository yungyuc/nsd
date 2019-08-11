#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>
#include <iostream>

template <class T>
class InstanceCounter
{

public:

    InstanceCounter() { ++m_constructed; }
    InstanceCounter(InstanceCounter const & other) { ++m_constructed; }
    ~InstanceCounter() { ++m_destructed; }

    static std::size_t active() { return m_constructed - m_destructed; }
    static std::size_t constructed() { return m_constructed; }
    static std::size_t destructed() { return m_destructed; }

private:

    static std::atomic_size_t m_constructed;
    static std::atomic_size_t m_destructed;

}; /* end class InstanceCounter */

template <class T> std::atomic_size_t InstanceCounter<T>::m_constructed = 0;
template <class T> std::atomic_size_t InstanceCounter<T>::m_destructed = 0;

struct Data
  : public InstanceCounter<Data>
{

    std::size_t buffer[1024];

}; /* end struct Data */

void report()
{
    using ct = InstanceCounter<Data>;

    std::cout
        << "instance: active = " << ct::active()
        << " constructed = " << ct::constructed()
        << " destructed = " << ct::destructed()
        << std::endl;
}

int main(int argc, char ** argv)
{
    Data * data = new Data();
    report();

    std::vector<Data> dvec(64);
    report();

    std::vector<Data>().swap(dvec);
    report();

    delete data;
    report();

    return -1;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
