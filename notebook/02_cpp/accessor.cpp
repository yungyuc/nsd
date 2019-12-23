#include <iostream>

class PointClass
{
    float m_x, m_y;
public:
    // Accessors: get/set style.
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setX(float v) { m_x = v; }
    void setY(float v) { m_y = v; }
    // Accessors of alternate style: single method name.
    float const & x() const { return m_x; } // getter
    float       & x()       { return m_x; } // setter
    float const & y() const { return m_y; } // getter
    float       & y()       { return m_y; } // setter
}; /* end class PointClass */

int main(int, char **)
{
    PointClass pntc;

    pntc.setX(2); pntc.setY(4);

    std::cout << "pntc.getX() = " << pntc.getX() << ", pntc.getY() = " << pntc.getY() << std::endl;

    pntc.x() = 12; pntc.y() = 24;

    std::cout << "pntc.x() = " << pntc.x() << ", pntc.y() = " << pntc.y() << std::endl;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
