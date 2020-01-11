# Find the xsimd package
# xsimd_INCLUDE_DIR
# xsimd_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.8.11)

find_path(xsimd_INCLUDE_DIR NAMES xsimd/xsimd.hpp
  HINTS ${xsimd_ROOT} ENV PATH PATH_SUFFIXES include)

if(xsimd_INCLUDE_DIR)
  message(STATUS "xsimd_INCLUDE_DIR: ${xsimd_INCLUDE_DIR}")
  set(xsimd_FOUND 1 CACHE INTERNAL "xsimd found")
else()
  message(STATUS "xsimd_INCLUDE_DIR: not found")
endif()