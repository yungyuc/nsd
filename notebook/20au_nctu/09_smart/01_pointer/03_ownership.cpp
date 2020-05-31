#include <iostream>
#include <cstdlib>
#include <algorithm>

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

Data * worker1()
{
    // Create a new Data object.
    Data * data = new Data();

    // Manipulate the Data object.
    manipulate_with_reference(*data);

    return data;
}

/*
 * Code in this function is intentionally made to be lack of discipline to
 * demonstrate how ownership is messed up.
 */
void worker2(Data * data)
{
    // The prerequisite for the caller to write correct code is to read the
    // code and understand when the object is alive.
    if (data->is_manipulated())
    {
        delete data;
    }
    else
    {
        manipulate_with_reference(*data);
    }
}

int main(int, char **)
{
    Data * data = worker1();
    std::cout << "Data pointer after worker 1: " << data << std::endl;
    worker2(data);
    std::cout << "Data pointer after worker 2: " << data << std::endl;

    // You have to read the code of worker2 to know that data could be
    // destructed.  In addition, the Data class doesn't provide a
    // programmatical way to detect whether or not the object is alive.  The
    // design of Data, worker1, and worker2 makes it impossible to write
    // memory-safe code.
#ifdef CRASHME // The fenced code causes double free.
    delete data;
    std::cout << "Data pointer after delete: " << data << std::endl;
#endif
}

// vim: set et sw=4 ts=4 sts=4:
