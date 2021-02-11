#include <iostream>
int main(int, char **)
{
    std::cout << "unit is byte" << std::endl;
    std::cout << "sizeof(char): " << sizeof(char) << std::endl;
    std::cout << "sizeof(short): " << sizeof(short) << std::endl;
    std::cout << "sizeof(int): " << sizeof(int) << std::endl;
    std::cout << "sizeof(long): " << sizeof(long) << std::endl;
    std::cout << "sizeof(long long): " << sizeof(long long) << std::endl;
    std::cout << "sizeof(unsigned char): " << sizeof(unsigned char) << std::endl;
    std::cout << "sizeof(unsigned short): " << sizeof(unsigned short) << std::endl;
    std::cout << "sizeof(unsigned int): " << sizeof(unsigned int) << std::endl;
    std::cout << "sizeof(unsigned long): " << sizeof(unsigned long) << std::endl;
    std::cout << "sizeof(unsigned long long): " << sizeof(unsigned long long) << std::endl;
    return 0;
}
