#include <pybind11/pybind11.h>

#include <cmath>
#include <tuple>

std::tuple<double, double> rotate(
    std::tuple<double, double> const & vec
  , double rad
)
{

    const double cth = cos(rad);
    const double sth = sin(rad);

    return std::tuple<double, double>(
        std::get<0>(vec) * cth - std::get<1>(vec) * sth
      , std::get<0>(vec) * sth + std::get<1>(vec) * cth);

}

PYBIND11_MODULE(_vector, mod)
{

    mod.doc() = "example C extension module";

    mod.def("rotate", &rotate, "vector rotation");

}

/* vim: set et ts=4 sw=4: */
