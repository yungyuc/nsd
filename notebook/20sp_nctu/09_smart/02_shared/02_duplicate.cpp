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

private:

    class ctor_passkey {};

public:

    Data() = delete;

    // TODO: Copyability and moveability should be considered, but we leave
    // them for now.

    static std::shared_ptr<Data> make()
    {
        std::shared_ptr<Data> ret = std::make_shared<Data>(ctor_passkey());
        return ret;
    }

    Data(ctor_passkey const &)
    {
        std::fill(begin(), end(), 0);
        std::cout << "Data @" << this << " is constructed" << std::endl;
    }

    ~Data()
    {
        std::cout << "Data @" << this << " is destructed" << std::endl;
    }

    Data * get_raw_ptr()
    {
        // Returning raw pointer discards the ownership management.
        return this;
    }

    std::shared_ptr<Data> get_shared_ptr()
    {
        // XXX: Recreate a shared_ptr will duplicate the reference counter, and
        // later results into double free.
        return std::shared_ptr<Data>(this);
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

int main(int, char **)
{
    std::shared_ptr<Data> data = Data::make();
    std::cout << "data.use_count(): " << data.use_count() << std::endl;

    std::shared_ptr<Data> holder2 = data->get_shared_ptr();

    data.reset();
    std::cout << "data.use_count() after data.reset(): " << data.use_count() << std::endl;

    std::cout << "holder2.use_count(): " << holder2.use_count() << std::endl;
    holder2.reset();
    // This line never gets reached since the the above line causes double free
    // and crash.
    std::cout << "holder2.use_count() after holder2.reset(): " << holder2.use_count() << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
