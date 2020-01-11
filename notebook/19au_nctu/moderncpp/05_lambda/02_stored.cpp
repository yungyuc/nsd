#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

int main(int argc, char ** argv)
{
    std::vector<int> data(63712);
    for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

    std::cout
        << "Number divisible by 23 (count by lambda inline): "
        << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%23; })
        << std::endl;

    auto condition = [](int v){ return 0 == v%23; };

    std::cout
        << "Number divisible by 23 (count by lambda in auto): "
        << std::count_if(data.begin(), data.end(), condition)
        << std::endl;

    std::function<bool (int)> condition_function = [](int v){ return 0 == v%23; };

    std::cout
        << "Number divisible by 23 (count by lambda in std::function): "
        << std::count_if(data.begin(), data.end(), condition_function)
        << std::endl;

#ifdef SHOW_DIFF
    // Difference between lambda and std::function.
    std::cout
        << std::endl
        << "The differences between lambda and std::function"
        << std::endl;
    std::cout
        << "type name of lambda: "
        << typeid(condition).name() << std::endl;
    std::cout
        << "type name of std::function: "
        << typeid(condition_function).name() << std::endl;

    std::cout
        << "size of lambda: "
        << sizeof(condition) << std::endl;
    std::cout
        << "size of std::function: "
        << sizeof(condition_function) << std::endl;
#endif

    return 0;
}

// vim: set et sw=4 ts=4 sts=4:
