#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Custom constructor.
    Line(size_t size) : m_size(size), m_coord(new float[size*2]) {}
    // Desctructor.
    ~Line() { if (nullptr != m_coord) { delete[] m_coord; } }
    // Accessors.
    size_t size() const { return m_size; }
    float & x(size_t it) const { check_range(it); return m_coord[it*2  ]; }
    float & x(size_t it)       { check_range(it); return m_coord[it*2  ]; }
    float & y(size_t it) const { check_range(it); return m_coord[it*2+1]; }
    float & y(size_t it)       { check_range(it); return m_coord[it*2+1]; }
private:
    // Private helper.
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
    // Member data.
    size_t m_size = 0; // number of points.
    float * m_coord = nullptr; // memory buffer for the points.
}; /* end class Line */

/* Define the copy constructor */
Line::Line(Line const & other)
{
    if (other.m_coord)
    {
        if (m_coord) { delete[] m_coord; } // free unused buffer.
        m_coord = new float[other.m_size*2];
        m_size = other.m_size;
        std::copy_n(other.m_coord, other.m_size*2, m_coord);
    }
    else // the other object is empty.
    {
        if (m_coord)
        {
            delete[] m_coord; // free unused buffer.
            m_coord = nullptr;
            m_size = 0;
        }
    }
}

/* Define the move constructor */
Line::Line(Line && other)
{
    std::swap(other.m_size, m_size);
    std::swap(other.m_coord, m_coord);
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    if (other.m_coord)
    {
        if (m_coord) { delete[] m_coord; }
        m_coord = new float[other.m_size*2];
        m_size = other.m_size;
        std::copy_n(other.m_coord, other.m_size*2, m_coord);
    }
    else
    {
        if (m_coord)
        {
            delete[] m_coord;
            m_coord = nullptr;
            m_size = 0;
        }
    }
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    std::swap(other.m_size, m_size);
    std::swap(other.m_coord, m_coord);
    return *this;
}

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
