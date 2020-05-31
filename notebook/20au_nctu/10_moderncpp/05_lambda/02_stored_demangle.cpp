#include <iostream>
#include <algorithm>
#include <vector>

// NOTE: This isn't guaranteed to work in every compiler.
#include <cxxabi.h>

std::string demangle(const char* name)
{
    // An arbitrary value to eliminate the compiler warning.
    int status = -1;

    std::unique_ptr<char, void(*)(void*)> res(
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    );

    return (status==0) ? res.get() : name;
}

int main(int argc, char ** argv)
{
    std::vector<int> data(63712);
    for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

    std::cout
        << "Number divisible by 23 (count by inline lambda): "
        << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%23; })
        << std::endl;

    auto condition = [](int v){ return 0 == v%23; };

    std::cout
        << "Number divisible by 23 (count by stored lambda): "
        << std::count_if(data.begin(), data.end(), condition)
        << std::endl;

    std::cout << "type name of condition: " << demangle(typeid(condition).name()) << std::endl;

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
