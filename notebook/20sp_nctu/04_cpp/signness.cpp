#include <iostream>
#include <cstdint>
int main(int, char **)
{
    long sint = -1;
    unsigned long uint = 1;
    std::cout << "sint: " << sint << std::endl;
    std::cout << "uint: " << uint << std::endl;
    if (sint > uint) { std::cout << "sint > uint, although it can't be" << std::endl; }
    return 0;
}
