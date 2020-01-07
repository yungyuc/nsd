# Find the xtl package
# xtl_INCLUDE_DIR
# xtl_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.8.11)

find_path(xtl_INCLUDE_DIR NAMES xtl/xtl_config.hpp
  HINTS ${xtl_ROOT} ENV PATH PATH_SUFFIXES include)

if(xtl_INCLUDE_DIR)
  message(STATUS "xtl_INCLUDE_DIR: ${xtl_INCLUDE_DIR}")
  set(xtl_FOUND 1 CACHE INTERNAL "xtl found")
else()
  message(STATUS "xtl_INCLUDE_DIR: not found")
endif()