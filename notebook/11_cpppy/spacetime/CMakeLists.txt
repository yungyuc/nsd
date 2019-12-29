# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>

cmake_minimum_required(VERSION 3.9)
project(spacetime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands")

option(BUILD_GTESTS "build libst google-test suite" ON)
option(HIDE_SYMBOL "hide the symbols of python wrapper" OFF)
option(DEBUG_SYMBOL "add debug information" ON)

message(STATUS "BUILD_GTESTS: ${BUILD_GTESTS}")
message(STATUS "HIDE_SYMBOL: ${HIDE_SYMBOL}")
message(STATUS "DEBUG_SYMBOL: ${DEBUG_SYMBOL}")

option(USE_CLANG_TIDY "use clang-tidy" OFF)
option(LINT_AS_ERRORS "clang-tidy warnings as errors" OFF)

if(DEBUG_SYMBOL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(PYLIBST_PATH "${PROJECT_SOURCE_DIR}/libst")

find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(USE_CLANG_TIDY)
    if(CLANG_TIDY_EXE)
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
        if(LINT_AS_ERRORS)
            set(DO_CLANG_TIDY "${DO_CLANG_TIDY}" "-warnings-as-errors=*")
        endif()
        message(STATUS "use clang-tidy with DO_CLANG_TIDY: ${DO_CLANG_TIDY}")
    else()
        message(FATAL_ERROR "USE_CLANG_TIDY is on but clang-tidy is not found")
    endif()
else()
    message(STATUS "not use clang-tidy")
endif()

option(pybind11_path "pybind11 path")
find_package(pybind11 REQUIRED PATHS ${pybind11_path})
message(STATUS "pybind11_INCLUDE_DIRS: ${pybind11_INCLUDE_DIRS}")
include_directories(${pybind11_INCLUDE_DIRS})

find_package(NumPy REQUIRED)
message(STATUS "NUMPY_INCLUDE_DIR: ${NUMPY_INCLUDE_DIR}")
include_directories(${NUMPY_INCLUDE_DIR})

find_package(xtl)
if(DEFINED xtl_FOUND)
  include_directories(BEFORE ${xtl_INCLUDE_DIR})
endif()

find_package(xsimd)
if(DEFINED xsimd_FOUND)
  include_directories(BEFORE ${xsimd_INCLUDE_DIR})
endif()

find_package(xtensor)
if(DEFINED xtensor_FOUND)
  include_directories(BEFORE ${xtensor_INCLUDE_DIR})
endif()

find_package(xtensor-python)
if(DEFINED xtensor-python_FOUND)
  include_directories(BEFORE ${xtensor-python_INCLUDE_DIR})
endif()

include_directories("include")

set(SPACETIME_HEADERS
    # Overall.
    include/spacetime.hpp
    # Framework.
    include/spacetime/Celm.hpp
    include/spacetime/Celm_decl.hpp
    include/spacetime/ElementBase.hpp
    include/spacetime/ElementBase_decl.hpp
    include/spacetime/Grid.hpp
    include/spacetime/Grid_decl.hpp
    include/spacetime/Field.hpp
    include/spacetime/Field_decl.hpp
    include/spacetime/Selm.hpp
    include/spacetime/Selm_decl.hpp
    include/spacetime/SolverBase.hpp
    include/spacetime/SolverBase_decl.hpp
    include/spacetime/Solver.hpp
    include/spacetime/io.hpp
    include/spacetime/system.hpp
    include/spacetime/type.hpp
    include/spacetime/math.hpp
    # Physical kernels.
    include/spacetime/kernel/linear_scalar.hpp
    include/spacetime/kernel/inviscid_burgers.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       SPACETIME_HEADERS "${SPACETIME_HEADERS}")

set(SPACETIME_PY_HEADERS
    include/spacetime/python.hpp
    include/spacetime/python/common.hpp
    include/spacetime/python/WrapBase.hpp
    include/spacetime/python/wrapper_spacetime.hpp
    include/spacetime/python/wrapper_linear_scalar.hpp
    include/spacetime/python/wrapper_inviscid_burgers.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       SPACETIME_PY_HEADERS "${SPACETIME_PY_HEADERS}")

set(LIBST_SOURCES
    src/python/libst.cpp
)
pybind11_add_module(
    _libst
    ${LIBST_SOURCES}
    ${SPACETIME_HEADERS}
    ${SPACETIME_PY_HEADERS}
)
if(HIDE_SYMBOL)
    set_target_properties(_libst PROPERTIES CXX_VISIBILITY_PRESET "hidden")
else()
    set_target_properties(_libst PROPERTIES CXX_VISIBILITY_PRESET "default")
endif()

add_custom_target(_libst_py
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_libst> ${PYLIBST_PATH}
    DEPENDS _libst)

if(CLANG_TIDY_EXE AND USE_CLANG_TIDY)
    set_target_properties(
        _libst PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
endif()

if(BUILD_GTESTS)
    add_subdirectory(gtests)
endif()

# vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4:
