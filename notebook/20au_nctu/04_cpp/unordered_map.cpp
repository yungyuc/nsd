#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

int main(int, char **)
{
    std::unordered_map<int, float> map1;
    for (size_t it=5; it != 0; --it) { map1[it] = 1./float(it); }

    std::cout << "map1:";
    for (std::pair<int, float> const p : map1)
    { std::cout << " (" << p.first << "," << p.second << ")"; }
    std::cout << std::endl;

    std::cout << "map1 " << (map1.count(3) ? "has " : "does not have ")
              << "key 3" << std::endl;
    std::cout << "map1 " << (map1.count(6) ? "has " : "does not have ")
              << "key 6" << std::endl;

    std::unordered_set<int> set1;
    for (size_t it=5; it != 0; --it) { set1.insert(it); }

    std::cout << "set1:";
    for (int const v : set1)
    { std::cout << " " << v; }
    std::cout << std::endl;

    std::cout << "set1 " << (set1.count(3) ? "has " : "does not have ")
              << "key 3" << std::endl;
    std::cout << "set1 " << (set1.count(6) ? "has " : "does not have ")
              << "key 6" << std::endl;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
