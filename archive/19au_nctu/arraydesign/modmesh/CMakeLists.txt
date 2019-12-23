# Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
# BSD-style license; see COPYING

cmake_minimum_required(VERSION 3.9)
project(modmesh)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands")

option(HIDE_SYMBOL "hide the symbols of python wrapper" OFF)
message(STATUS "HIDE_SYMBOL: ${HIDE_SYMBOL}")

option(DEBUG_SYMBOL "add debug information" ON)
message(STATUS "DEBUG_SYMBOL: ${DEBUG_SYMBOL}")
if(DEBUG_SYMBOL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

option(MODMESH_PROFILE "enable profiler" OFF)
message(STATUS "MODMESH_PROFILE: ${MODMESH_PROFILE}")
if(MODMESH_PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMODMESH_PROFILE")
endif()

option(pybind11_path "pybind11 path")
find_package(pybind11 REQUIRED PATHS ${pybind11_path})
message(STATUS "pybind11_INCLUDE_DIRS: ${pybind11_INCLUDE_DIRS}")
include_directories(${pybind11_INCLUDE_DIRS})

find_package(NumPy REQUIRED)
message(STATUS "NUMPY_INCLUDE_DIR: ${NUMPY_INCLUDE_DIR}")
include_directories(${NUMPY_INCLUDE_DIR})

include_directories("include")

set(MODMESH_HEADERS
    include/modmesh/modmesh.hpp
    include/modmesh/base.hpp
    include/modmesh/profile.hpp
    include/modmesh/grid.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       MODMESH_HEADERS "${MODMESH_HEADERS}")

set(MODMESH_PY_HEADERS
    include/modmesh/python/python.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       MODMESH_PY_HEADERS "${MODMESH_PY_HEADERS}")

set(MODMESH_SOURCES
    src/modmesh.cpp
)
pybind11_add_module(
    _modmesh
    ${MODMESH_SOURCES}
    ${MODMESH_HEADERS}
    ${MODMESH_PY_HEADERS}
)
if(HIDE_SYMBOL)
    set_target_properties(_modmesh PROPERTIES CXX_VISIBILITY_PRESET "hidden")
else()
    set_target_properties(_modmesh PROPERTIES CXX_VISIBILITY_PRESET "default")
endif()

set(MODMESH_PY_PATH "${PROJECT_SOURCE_DIR}/modmesh")

add_custom_target(_modmesh_py
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_modmesh> ${MODMESH_PY_PATH}
    DEPENDS _modmesh)

# vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4 sts=4:
