#include <iostream>
#include <cfenv>
#include <cmath>
#include <limits>
int main(int, char **)
{
    float v1;

    feclearexcept(FE_ALL_EXCEPT);
    v1 = 0.3;
    std::cout << "result: " << v1/0 << std::endl;
    if (fetestexcept(FE_DIVBYZERO)) { std::cout << "  FE_DIVBYZERO" << std::endl; }

    feclearexcept(FE_ALL_EXCEPT);
    v1 = 2;
    std::cout << "std::sqrt(2): " << std::sqrt(v1) << std::endl;
    if (fetestexcept(FE_INEXACT)) { std::cout << "  FE_INEXACT" << std::endl; }

    feclearexcept(FE_ALL_EXCEPT);
    v1 = 2;
    std::cout << "std::acos(2): " << std::acos(v1) << std::endl;
    if (fetestexcept(FE_INVALID)) { std::cout << "  FE_INVALID" << std::endl; }

    feclearexcept(FE_ALL_EXCEPT);
    v1 = std::numeric_limits<float>::max();
    std::cout << "std::numeric_limits<float>::max() * 2: " << v1 * 2 << std::endl;
    if (fetestexcept(FE_OVERFLOW)) { std::cout << "  FE_OVERFLOW" << std::endl; }

    feclearexcept(FE_ALL_EXCEPT);
    v1 = std::numeric_limits<float>::min();
    std::cout << "std::numeric_limits<float>::min() / 10: " << v1 / 10 << std::endl;
    if (fetestexcept(FE_UNDERFLOW)) { std::cout << "  FE_UNDERFLOW" << std::endl; }

    return 0;
}
