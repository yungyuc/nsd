# Find the xtensor package
# xtensor_INCLUDE_DIR
# xtensor_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.8.11)

find_path(xtensor_INCLUDE_DIR NAMES xtensor/xtensor.hpp
  HINTS ${xtensor_ROOT} ENV PATH PATH_SUFFIXES include)

if(xtensor_INCLUDE_DIR)
  message(STATUS "xtensor_INCLUDE_DIR: ${xtensor_INCLUDE_DIR}")
  set(xtensor_FOUND 1 CACHE INTERNAL "xtensor found")
else()
  message(STATUS "xtensor_INCLUDE_DIR: not found")
endif()