#include <iostream>
#include "cblas.h"
int main(int, char **)
{
    double vx[] = {1, 2};
    double vy[] = {-2, 1};
    std::cout << "x = (" << vx[0] << ", " << vx[1] << ")" << std::endl;
    std::cout << "y = (" << vy[0] << ", " << vy[1] << ")" << std::endl;
    std::cout << "x \\dot y = " << cblas_ddot(2, vx, 1, vy, 1) << std::endl;
    return -1;
}
