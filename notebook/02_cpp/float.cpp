#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
int main(int, char **)
{
    float fvalue;
    std::bitset<32> b32value;

    fvalue = 0.3;
    b32value = *reinterpret_cast<uint32_t *>(&fvalue);
    std::cout << "fvalue: " << std::setprecision(10) << fvalue << std::endl;
    std::cout << "b32value (float sep):   ";
    std::cout << " "; for (size_t it=0; it<1; ++it) { std::cout << b32value[31-it]; }
    std::cout << " "; for (size_t it=1; it<9; ++it) { std::cout << b32value[31-it]; }
    std::cout << " "; for (size_t it=9; it<32; ++it) { std::cout << b32value[31-it]; }
    std::cout << std::endl;
    std::cout << "                      sign exponent fraction" << std::endl;

    fvalue = 3;
    b32value = *reinterpret_cast<uint32_t *>(&fvalue);
    std::cout << "fvalue: " << std::setprecision(10) << fvalue << std::endl;
    std::cout << "b32value (float sep):   ";
    std::cout << " "; for (size_t it=0; it<1; ++it) { std::cout << b32value[31-it]; }
    std::cout << " "; for (size_t it=1; it<9; ++it) { std::cout << b32value[31-it]; }
    std::cout << " "; for (size_t it=9; it<32; ++it) { std::cout << b32value[31-it]; }
    std::cout << std::endl;
    std::cout << "                      sign exponent fraction" << std::endl;

    return 0;
}
