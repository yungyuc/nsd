#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

int main(int argc, char ** argv)
{
    std::vector<int> data(63712);
    for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

    int divisor = 23;

#if WRONG_CAPTURE
    std::cout
        << "Count (wrong capture): "
        << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%divisor; })
        << " (divisor: " << divisor << ")"
        << std::endl;
#endif

    std::cout
        << "Count (lambda explicitly capture by value): "
        << std::count_if(data.begin(), data.end(), [divisor](int v){ return 0 == v%divisor; })
        << " (divisor: " << divisor << ")"
        << std::endl;

    std::cout
        << "Count (lambda implicitly capture by value): "
        << std::count_if(data.begin(), data.end(), [=](int v){ return 0 == v%divisor; })
        << " (divisor: " << divisor << ")"
        << std::endl;

    std::cout
        << "Count (lambda explicitly capture by reference): "
        << std::count_if(data.begin(), data.end(), [&divisor](int v){ divisor = 10; return 0 == v%divisor; })
        << " (divisor: " << divisor << ")"
        << std::endl;

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
