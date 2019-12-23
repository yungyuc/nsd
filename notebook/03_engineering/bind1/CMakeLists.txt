cmake_minimum_required(VERSION 3.9)
project(vector)

find_package(pybind11 REQUIRED)

pybind11_add_module(
    _vector
    vector.cpp
)

install(TARGETS _vector DESTINATION .)
