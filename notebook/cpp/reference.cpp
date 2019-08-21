#include <iostream>
int main(int, char **)
{
    int v = 10;
    std::cout << "v = " << v << std::endl;
    int * pv; // pointer; danger: uninitialized
    pv = &v;
    *pv = 11;
    std::cout << "v = " << v << std::endl;
    int & rv = v; // reference
    rv = 12;
    std::cout << "v = " << v << std::endl;
    // error: declaration of reference variable 'nrv' requires an initializer
    //int & nrv;
    int const & crv = v; // const reference
    // error: cannot assign to variable 'crv' with const-qualified type 'const int &'
    //crv = 12;
    std::cout << "&v, pv, &rv, &crv (address): " << std::endl
              << "  " << &v << std::endl
              << "  " << pv << std::endl
              << "  " << &rv << std::endl
              << "  " << &crv << std::endl;
    return 0;
}
// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
