#include <cstdint>
#include <algorithm>
#include <utility>
#include <cmath>
#include <iostream>

template <class Derived>
class PointBase
{
public:
    constexpr static const double PI = 3.14159265358979323846;
    PointBase(float v0, float v1) : m_v0(v0), m_v1(v1) {}
    float const & v0() const { return m_v0; }
    float       & v0()       { return m_v0; }
    float const & v1() const { return m_v1; }
    float       & v1()       { return m_v1; }
    float dist() const
    {
        // Prevent the derived class from working if it doesn't define dist(),
        // just like what a pure virtual function does.
        static_assert(&PointBase<Derived>::dist != &Derived::dist,
                      "derived class must define dist()");
        return derived().dist();
    }
private:
    Derived const & derived() const { return *static_cast<Derived const *>(this); }
    float m_v0, m_v1;
}; /* end class PointBase */

class CartesianPoint : public PointBase<CartesianPoint>
{
public:
    using base_type = PointBase<CartesianPoint>;
    using base_type::base_type;
    float dist() const
    {
        return std::hypot(v0(), v1());
    }
}; /* end class CartesianPoint */

class PolarPoint : public PointBase<PolarPoint>
{
public:
    using base_type = PointBase<PolarPoint>;
    using base_type::base_type;
    float dist() const
    {
        return std::abs(v0());
    }
}; /* end class PolarPoint */

class BadPoint : public PointBase<BadPoint>
{
public:
    using base_type = PointBase<BadPoint>;
    using base_type::base_type;
    // Intentionally omit dist().
    /*float dist() const
    {
        return 0.0;
    }*/
}; /* end class BadPoint */

int main(int, char **)
{
    std::cout << "sizeof(CartesianPoint) = " << sizeof(CartesianPoint) << std::endl;
    std::cout << "sizeof(PolarPoint) = " << sizeof(PolarPoint) << std::endl;

    CartesianPoint cpt(1, 1);
    PolarPoint ppt(1, PolarPoint::PI / 4);

    std::cout << "CartesianPoint(1,1)::dist() = " << cpt.dist() << std::endl;
    std::cout << "PolarPoint(1, pi/4)::dist() = " << ppt.dist() << std::endl;

    // This doesn't build.
    // error: static_assert failed "derived class must define dist()"
    //std::cout << "BadPoint(1, 1)::dist() = " << BadPoint(1,1).dist() << std::endl;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
