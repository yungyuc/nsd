#include <cstdint>
#include <stdexcept>
#include <list>
#include <iostream>

int main(int, char **)
{
    // Populate lst1.
    std::list<int> lst1;
    for (size_t it=0; it<3; ++it) { lst1.push_back(100+it); }
    std::cout << "lst1:";
    for (int v : lst1) { std::cout << " " << v; }
    std::cout << std::endl;

    // Populate lst2.
    std::list<int> lst2;
    for (size_t it=0; it<3; ++it) { lst2.push_front(200+it); }
    std::cout << "lst2:";
    for (int v : lst2) { std::cout << " " << v; }
    std::cout << std::endl;

    // Use an iterator to search for an element.
    std::list<int>::iterator it;
    for (it=lst2.begin(); it != lst2.end(); ++it)
    { if (*it == 201) { break; } }

    // Erase the found element, and then insert a new one.
    lst2.erase(it++);
    lst2.insert(it, 301);

    std::cout << "lst2 (modified):";
    for (int v : lst2) { std::cout << " " << v; }
    std::cout << std::endl;

    // Transfer elements from lst1 to lst2 (splice).
    lst2.splice(it, lst1, ++lst1.begin(), lst1.end());
    std::cout << "lst1 (after splice):";
    for (int v : lst1) { std::cout << " " << v; }
    std::cout << std::endl;
    std::cout << "lst2 (after splice):";
    for (int v : lst2) { std::cout << " " << v; }
    std::cout << std::endl;

    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
