#include <iostream>
#include <cstdint>
#include <limits>
int main(int, char **)
{
    std::cout << "type\t\tlowest()\tmin()\t\tmax()\t\tepsilon()" << std::endl << std::endl;
    std::cout << "float\t\t"
              << std::numeric_limits<float>::lowest() << "\t"
              << std::numeric_limits<float>::min() << "\t"
              << std::numeric_limits<float>::max() << "\t"
              << std::numeric_limits<float>::epsilon() << "\t"
              << std::endl;
    std::cout << "double\t\t"
              << std::numeric_limits<double>::lowest() << "\t"
              << std::numeric_limits<double>::min() << "\t"
              << std::numeric_limits<double>::max() << "\t"
              << std::numeric_limits<double>::epsilon() << "\t"
              << std::endl;
    std::cout << "int32_t\t\t"
              << std::numeric_limits<int32_t>::lowest() << "\t"
              << std::numeric_limits<int32_t>::min() << "\t"
              << std::numeric_limits<int32_t>::max() << "\t"
              << std::numeric_limits<int32_t>::epsilon() << "\t"
              << std::endl;
    std::cout << "uint32_t\t"
              << std::numeric_limits<uint32_t>::lowest() << "\t\t"
              << std::numeric_limits<uint32_t>::min() << "\t\t"
              << std::numeric_limits<uint32_t>::max() << "\t"
              << std::numeric_limits<uint32_t>::epsilon() << "\t"
              << std::endl;
    std::cout << "int64_t\t\t"
              << std::numeric_limits<int64_t>::lowest() << "\t"
              << std::numeric_limits<int64_t>::min() << "\t"
              << std::numeric_limits<int64_t>::max() << "\t"
              << std::numeric_limits<int64_t>::epsilon() << "\t"
              << std::endl;
    std::cout << "uint64_t\t"
              << std::numeric_limits<uint64_t>::lowest() << "\t\t"
              << std::numeric_limits<uint64_t>::min() << "\t\t"
              << std::numeric_limits<uint64_t>::max() << "\t"
              << std::numeric_limits<uint64_t>::epsilon() << "\t"
              << std::endl;
    return 0;
}
