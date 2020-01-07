#include <iostream>
#include <cstdint>
int main(int, char **)
{
    std::cout << "unit is byte" << std::endl;
    std::cout << "sizeof(int8_t): " << sizeof(int8_t) << std::endl;
    std::cout << "sizeof(uint8_t): " << sizeof(uint8_t) << std::endl;
    std::cout << "sizeof(int16_t): " << sizeof(int16_t) << std::endl;
    std::cout << "sizeof(uint16_t): " << sizeof(uint16_t) << std::endl;
    std::cout << "sizeof(int32_t): " << sizeof(int32_t) << std::endl;
    std::cout << "sizeof(uint32_t): " << sizeof(uint32_t) << std::endl;
    std::cout << "sizeof(int64_t): " << sizeof(int64_t) << std::endl;
    std::cout << "sizeof(uint64_t): " << sizeof(uint64_t) << std::endl;
    return 0;
}
