#include <cstdint>
#include <algorithm>
#include <utility>
#include <cmath>
#include <iostream>

class Line
{
public:
    Line() = default;
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size) : m_size(size), m_coord(new float[size*2]) {}
    virtual ~Line() { if (nullptr != m_coord) { delete[] m_coord; } }
    virtual float length() const;
    size_t size() const { return m_size; }
    float & x(size_t it) const { check_range(it); return m_coord[it*2  ]; }
    float & x(size_t it)       { check_range(it); return m_coord[it*2  ]; }
    float & y(size_t it) const { check_range(it); return m_coord[it*2+1]; }
    float & y(size_t it)       { check_range(it); return m_coord[it*2+1]; }
private:
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
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

float Line::length() const
{
    float ret = 0;
    for (size_t it=1; it<size(); ++it) {
        ret += std::hypot(x(it) - x(it-1), y(it) - y(it-1));
    }
    return ret;
}

class WeighedLine : public Line
{
public:
    WeighedLine(WeighedLine const & );
    WeighedLine(WeighedLine       &&);
    WeighedLine & operator=(WeighedLine const & );
    WeighedLine & operator=(WeighedLine       &&);
    WeighedLine(size_t size) : Line(size), m_weight(new float[size-1]) {}
    virtual ~WeighedLine() override { delete[] m_weight; }
    virtual float length() const override;
    float const & weight(size_t it) const { return m_weight[it]; }
    float       & weight(size_t it)       { return m_weight[it]; }
private:
    float * m_weight = nullptr; // weight on line segments.
}; /* end class WeighedLine */

WeighedLine::WeighedLine(WeighedLine const & other)
  : Line(other)
{
    if (other.m_weight)
    {
        if (m_weight) { delete[] m_weight; } // free unused buffer.
        m_weight = new float[other.size()-1];
        std::copy_n(other.m_weight, other.size()-1, m_weight);
    }
    else // the other object is empty.
    {
        if (m_weight)
        {
            delete[] m_weight; // free unused buffer.
            m_weight = nullptr;
        }
    }
}

WeighedLine::WeighedLine(WeighedLine && other)
  : Line(other)
{
    std::swap(other.m_weight, m_weight);
}

WeighedLine & WeighedLine::operator=(WeighedLine const & other)
{
    if (this == &other) { return *this; }
    Line::operator=(other);
    if (other.m_weight)
    {
        if (m_weight) { delete[] m_weight; } // free unused buffer.
        m_weight = new float[other.size()-1];
        std::copy_n(other.m_weight, other.size()-1, m_weight);
    }
    else // the other object is empty.
    {
        if (m_weight)
        {
            delete[] m_weight; // free unused buffer.
            m_weight = nullptr;
        }
    }
    return *this;
}

WeighedLine & WeighedLine::operator=(WeighedLine && other)
{
    if (this == &other) { return *this; }
    Line::operator=(other);
    std::swap(other.m_weight, m_weight);
    return *this;
}

float WeighedLine::length() const
{
    float ret = 0;
    for (size_t it=1; it<size(); ++it) {
        ret += weight(it-1) * std::hypot(x(it) - x(it-1), y(it) - y(it-1));
    }
    return ret;
}

int main(int, char **)
{
    std::cout << "sizeof(Line) = " << sizeof(Line) << std::endl;
    std::cout << "sizeof(WeighedLine) = " << sizeof(WeighedLine) << std::endl;

    WeighedLine line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 5; line.y(1) = 1; line.weight(0) = 1;
    line.x(2) = 5; line.y(2) = 4; line.weight(1) = 2;

    Line line2(line);
    line2.x(0) = 2;

    WeighedLine * pwline3 = new WeighedLine(line);
    Line * pline3 = pwline3;

    // Without runtime information the compiler cannot know whether or not downcasting is safe.
    // error: assigning to 'WeighedLine *' from incompatible type 'Line *'
    //pwline3 = pline3;

    // RTTI knows pline3 points to a WeighedLine object, so this downcasting is fine.
    pwline3 = dynamic_cast<WeighedLine *>(pline3);
    std::cout << "downcasting from Line * to WeighedLine * works on a WeighedLine object: pwline3= " << pwline3 << std::endl;

    WeighedLine * pwline2;
    pwline2 = dynamic_cast<WeighedLine *>(&line2);
    std::cout << "downcasting from Line * to WeighedLine * fails on a Line object: pwline2 = " << pwline2 << std::endl;

    pwline3->x(0) = 3;

    std::cout << "Object type of pwline3: " << typeid(*pwline3).name() << std::endl;
    std::cout << "Object type of pline3: " << typeid(*pline3).name() << std::endl;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it);
        if (it != 0)
        {
            std::cout << " weight = " << line.weight(it-1);
        }
        std::cout << std::endl;
    }
    std::cout << "  length = " << line.length() << std::endl;

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it);
        // This doesn't build.
        /*if (it != 0)
        {
            // error: no member named 'weight' in 'Line'
            std::cout << " weight = " << line2.weight(it-1);
        }*/
        std::cout << std::endl;
    }
    std::cout << "  length = " << line2.length() << std::endl;

    Line & line3 = *pline3;

    std::cout << "line3: number of points = " << line3.size() << std::endl;
    for (size_t it=0; it<line3.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line3.x(it)
                  << " y = " << line3.y(it);
        // This doesn't build.
        /*if (it != 0)
        {
            std::cout << " weight = " << line3.weight(it-1);
        }*/
        std::cout << std::endl;
    }
    std::cout << "  length = " << line3.length() << std::endl;

    WeighedLine & wline3 = *pwline3;

    std::cout << "wline3: number of points = " << wline3.size() << std::endl;
    for (size_t it=0; it<wline3.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << wline3.x(it)
                  << " y = " << wline3.y(it);
        if (it != 0)
        {
            std::cout << " weight = " << wline3.weight(it-1);
        }
        std::cout << std::endl;
    }
    std::cout << "  length = " << wline3.length() << std::endl;

    delete pline3;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
