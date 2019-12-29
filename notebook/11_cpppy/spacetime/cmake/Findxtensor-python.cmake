# Find the xtensor-python package
# xtensor-python_INCLUDE_DIR
# xtensor-python_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.8.11)

find_path(xtensor-python_INCLUDE_DIR NAMES xtensor-python/pyarray.hpp
  HINTS ${xtensor-python_ROOT} ENV PATH PATH_SUFFIXES include)

if(xtensor-python_INCLUDE_DIR)
  message(STATUS "xtensor-python_INCLUDE_DIR: ${xtensor-python_INCLUDE_DIR}")
  set(xtensor-python_FOUND 1 CACHE INTERNAL "xtensor-python found")
else()
  message(STATUS "xtensor-python_INCLUDE_DIR: not found")
endif()