#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>

class Data
{

public:

    constexpr const static size_t NELEM = 1024*8;

    using iterator = int *;
    using const_iterator = const int *;

    Data()
    {
        std::fill(begin(), end(), 0);
        std::cout << "Data @" << this << " is constructed" << std::endl;
    }

    ~Data()
    {
        std::cout << "Data @" << this << " is destructed" << std::endl;
    }

    const_iterator cbegin() const { return m_buffer; }
    const_iterator cend() const { return m_buffer+NELEM; }
    iterator begin() { return m_buffer; }
    iterator end() { return m_buffer+NELEM; }

    size_t size() const { return NELEM; }
    int   operator[](size_t it) const { return m_buffer[it]; }
    int & operator[](size_t it)       { return m_buffer[it]; }

    bool is_manipulated() const
    {
        for (size_t it=0; it < size(); ++it)
        {
            const int v = it;
            if ((*this)[it] != v) { return false; }
        }
        return true;
    }

private:

    // A lot of data that we don't want to reconstruct.
    int m_buffer[NELEM];

}; /* end class Data */

void manipulate_with_reference(Data & data)
{
    std::cout << "Manipulate with reference: " << &data << std::endl;

    for (size_t it=0; it < data.size(); ++it)
    {
        data[it] = it;
    }
    // In a real consumer function we will do much more meaningful operations.

    // However, we cannot destruct an object passed in with a reference.
}

static_assert(sizeof(Data *) < sizeof(std::shared_ptr<Data>), "shared_ptr uses more than a word");

std::shared_ptr<Data> worker1()
{
    // Create a new Data object.
    std::shared_ptr<Data> data = std::make_shared<Data>();

    std::cout << "worker 1 data.use_count(): " << data.use_count() << std::endl;

    // Manipulate the Data object.
    manipulate_with_reference(*data);

    return data;
}

void worker2(std::shared_ptr<Data> data)
{
    std::cout << "worker 2 data.use_count(): " << data.use_count() << std::endl;

    if (data->is_manipulated())
    {
        data.reset();
    }
    else
    {
        manipulate_with_reference(*data);
    }
}

int main(int, char **)
{
    std::shared_ptr<Data> data = worker1();
    std::cout << "Data pointer after worker 1: " << data.get() << std::endl;

    worker2(data);
    std::cout << "Data pointer after worker 2: " << data.get() << std::endl;

    data.reset();
    std::cout << "Data pointer after reset from outside: " << data.get() << std::endl;
    std::cout << "main data.use_count(): " << data.use_count() << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
