#include <iostream>

class PointClass
{
    float m_x, m_y; // by default private.
public:
    // Accessors.
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setX(float v) { m_x = v; }
    void setY(float v) { m_y = v; }
}; /* end class PointClass */

struct PointStruct
{
    float m_x, m_y; // by default public.
}; /* end class PointStruct */

int main(int, char **)
{
    PointClass pntc;
    PointStruct pnts;

    float x=1000, y=2000;

    pntc.setX(x+2); pntc.setY(y+4);
    pnts.m_x = x+7; pnts.m_y = x+9;

    std::cout << "PointClass and PointStruct has the same size: "
              << (sizeof(PointClass) == sizeof(PointStruct) ? "true" : "false")
              << std::endl;

    std::cout << "pntc.getX() = " << pntc.getX() << ", pntc.getY() = " << pntc.getY() << std::endl;

    std::cout << "pnts.m_x = " << pnts.m_x << ", pnts.m_y = " << pnts.m_y << std::endl;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
