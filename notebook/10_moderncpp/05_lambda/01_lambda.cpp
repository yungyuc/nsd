#include <iostream>
#include <algorithm>
#include <vector>

struct Functor
{
    bool operator()(int v)
    {
        return 0 == v % 23;
    }
}; /* end struct Functor */

int main(int argc, char ** argv)
{
    std::vector<int> data(63712);
    for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

    std::cout
        << "Number divisible by 23 (count by functor): "
        << std::count_if(data.begin(), data.end(), Functor())
        << std::endl;

    std::cout
        << "Number divisible by 23 (count by lambda): "
        << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%23; })
        << std::endl;

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
