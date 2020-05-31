#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>

class Child;

class Data
  : public std::enable_shared_from_this<Data>
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

    std::shared_ptr<Child>   child() const { return m_child; }
    std::shared_ptr<Child> & child()       { return m_child; }

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

    std::shared_ptr<Child> m_child;

}; /* end class Data */

class Child
  : public std::enable_shared_from_this<Child>
{

private:

    class ctor_passkey {};

public:

    Child() = delete;

    Child(std::shared_ptr<Data> const & data, ctor_passkey const &) : m_data(data) {}

    static std::shared_ptr<Child> make(std::shared_ptr<Data> const & data)
    {
        std::shared_ptr<Child> ret = std::make_shared<Child>(data, ctor_passkey());
        data->child() = ret;
        return ret;
    }

private:

    // Replace shared_ptr with weak_ptr to Data.
    std::weak_ptr<Data> m_data;

}; /* end class Child */

int main(int, char **)
{
    std::shared_ptr<Data> data = Data::make();
    std::shared_ptr<Child> child = Child::make(data);
    std::cout << "data.use_count(): " << data.use_count() << std::endl;
    std::cout << "child.use_count(): " << child.use_count() << std::endl;

    std::weak_ptr<Data> wdata(data);
    std::weak_ptr<Child> wchild(child);

    child.reset();
    std::cout << "wdata.use_count() after child.reset(): " << wdata.use_count() << std::endl;
    std::cout << "wchild.use_count() after child.reset(): " << wchild.use_count() << std::endl;

    data.reset();
    std::cout << "wdata.use_count() after data.reset(): " << wdata.use_count() << std::endl;
    std::cout << "wchild.use_count() after data.reset(): " << wchild.use_count() << std::endl;
}

// vim: set et sw=4 ts=4 sts=4:
