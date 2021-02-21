# Numerical Software Development Course Notes

Copyright 2021, Yung-Yu Chen <yyc@solvcon.net>.  All rights reserved.

![Build Status](https://github.com/yungyuc/nsd/workflows/primary/badge.svg)

**Objectives**: This course discusses the art to build numerical software, i.e., computer programs applying numerical methods for solving mathematical or physical problems.  We will be using the combination of Python and C++ and related tools (e.g., [bash](https://www.gnu.org/software/bash/), [git](https://git-scm.com), [make](https://www.gnu.org/software/make/), etc.) to learn the modern development processes.  By completing this course, students will acquire the fundamental skills for developing modern numerical software.

**Prerequisites**:  This is a graduate or senior level course open to students who have taken computer architecture, engineering mathematics or equivalents.  Working knowledge of Linux and Unix-like is required.  Prior knowledge to numerical methods is recommended.  The instructor uses English in the lectures and discussions.

# Course design

* You are expected to learn programming languages yourself.  Python is never a problem, but you could find it challenging to self-teach C++.  Students are encouraged to form study groups for practicing C++, and discuss with the instructor and/or the teaching assistant.
* Grading: homework 30%, mid-term exam: 30%, term project: 40%.
* There are 12 lectures for the subjects of numerical software developing using Python and C++.
* There will be 6 homework assignments for you to exercise.  Programming in Python and/or C++ is required.
* Mid-term examination will be conducted to assess students' understandings to the analytical materials.
* Term project will be used to assess students' overall coding skills.  Presentation is required.  Failure to present results in **0 point** for this part.  Check [the term project page](https://github.com/yungyuc/nsd/blob/master/notebook/current/term_project.ipynb) before you start.

# How to study

* This is a practical course.  No textbook is available for this specific interdisciplinary subject.
* To study the subject, students are required to research with online documents and source code, and write programs to practice.
* In-class instruction and [course notes](https://yungyuc.github.com/nsd) ([github](https://github.com/yungyuc/nsd/blob/master/notebook/current/index.ipynb)) are provided for guidance.
* References:
  * Computer Systems: A Programmer's Perspective, Randal E. Bryant and David R. O'Hallaron: https://csapp.cs.cmu.edu/
  * Python documentation: https://docs.python.org/3/
  * Cppreference: https://en.cppreference.com/
  * [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/), Scott Meyer, O'Reilly, 2014
  * Source code: [cpython](https://github.com/python/cpython), [numpy](https://github.com/numpy/numpy), [xtensor](https://github.com/QuantStack/xtensor), and [pybind11](https://github.com/pybind/pybind11)

# Course schedule

* W1 (2/22) Unit 1 and Unit 2: Introduction and fundamental engineering practices (*homework \#1*)
* W2 (3/1) Skip meeting (peace memorial day)
* W3 (3/8) Unit 3: Python and numpy (*term project proposal start*)
* W4 (3/15) Unit 4: C++ and computer architecture (*homework \#2*)
* W5 (3/22) Unit 5: Matrix operations
* W6 (3/29) Unit 6: Cache optimization (*homework \#3*)
* W7 (4/5) Skip meeting (spring holiday) (*term project proposal due*)
* M8 (4/12) Unit 7: SIMD
* W9 (4/19) Mid-term examination
* W10 (4/26) Unit 8: Memory management (*homework \#4*)
* W11 (5/3) Unit 9: Ownership and smart pointers
* W12 (5/10) Unit 10: Modern C++ (*homework \#5*)
* W13 (5/17) Unit 11: C++ and C for Python
* W14 (5/24) Unit 12: Array code in C++ (*homework \#6*)
* W15 (5/31) Unit 13: Advanced Python
* W16 (6/7) Term project presentation
* W17 (6/14) No meeting
* W18 (6/21) No meeting

# Unit 1 [Introduction](https://github.com/yungyuc/nsd/blob/master/notebook/current/01_introduction/introduction.ipynb)

1. Part 1: What is numerical software
   1. Why develop numerical software
   2. Hybrid architecture
   3. Numerical software = C++ + Python
2. Part 2: What to learn in this course
   1. Term project
   2. How to write a proposal
   3. Term project grading guideline
   4. Online discussion
3. Part 3: Runtime and course marterials
   1. Runtime environment: Linux and AWS
   2. Jupyter notebook

# Unit 2 [Fundamental engineering practices](https://github.com/yungyuc/nsd/blob/master/notebook/current/02_engineering/engineering.ipynb)

A large chunk of efforts is spent in the infrastructure for coding.  The key to the engineering system is automation.

1. Automation
   1. Bash scripting
   2. Makefile
   3. Cmake (cross-platform, multi-language automation)
2. Version control and regression
   1. Git version control system
   2. Automatic testing: author and run with google-test and py.test
   3. Wrap to Python and test there: pybind11
   4. Continuous integration to avoid regression
3. Work that cannot be automated
   1. Code review (use github for demonstration)
   2. Timing to debug for performance
      1. Wall time and CPU cycles
      2. System time and user time
      3. Python timing tools

# Unit 3 [Python and numpy](https://github.com/yungyuc/nsd/blob/master/notebook/current/03_numpy/numpy.ipynb)

Python is a popular choice for the scripting engine that makes the numerical software work as a platform.

The platform works like a library providing data structures and helpers to solve problems.  The users will use Python to build applications.

1. Organize Python modules
   1. Scripts
   2. Modules
   3. Packages
2. Use numpy for array-oriented code
   1. Data type
   2. Construction
   3. Multi-dimensional arrays
   4. Selection
   5. Broadcasting
3. Use tools for numerical analysis
   1. Matplotlib
   2. Linear algebra using numpy and scipy
   3. Package management wtih conda and pip

# Unit 4 [C++ and computer architecture](https://github.com/yungyuc/nsd/blob/master/notebook/current/04_cpp/cpp.ipynb)

The low-level code of numerical software must be high-performance.  The industries chose C++ because it can take advantage of everything that a hardware architecture offers while using any level of abstraction.

1. Fundamental data types
   1. Command-line interface for compiler tools
      1. Compiler, linker
      2. Multiple source files, separation of declaration and definition, external libraries
      3. Build multiple binaries and shared objects (dynamically linked libraries)
   2. Integer, signness, pointer, array indexing
   3. Floating-point, rounding, exception handling
   4. Numeric limit
2. Object-oriented programming
   1. Class, encapsulation, accessor, reference type
   2. Constructor and destructor
   3. Polymorphism and RTTI
   4. CRTP
3. Standard template library (STL)
   1. std::vector, its internal and why the buffer address is dangerous
   2. std::array, std::list
   3. std::map, std::set, std::unordered_map, std::unordered_set

# Unit 5 [Matrix operations](https://github.com/yungyuc/nsd/blob/master/notebook/current/05_matrix/matrix.ipynb)

Matrices are everywhere in numerical analysis.  Arrays are the fundamental data structure and used for matrix-vector, matrix-matrix, and other linear algebraic operations.

1. POD arrays and majoring
   1. Vector: 1D array
   1. Matrix: 2D array
   1. Row- and column-majoring
   1. A simple class for matrix
2. Matrix-vector and matrix-matrix operations
   1. Matrix-vector multiplication
   1. Matrix-matrix multiplication
3. Linear algebra
   1. Linear system solution
   1. Eigenvalue and singular value problems
   1. Least square problems

# Unit 6 [Cache optimization](https://github.com/yungyuc/nsd/blob/master/notebook/current/06_cache/cache.ipynb)

How cache works, its importance to performance, and optimization with cache.

1. Memory hierarchy
2. How cache works
   1. Cache block (line) size determines speed
   2. Locality
   3. Matrix population in C++
   4. Array majoring in numpy
3. Tiling

# Unit 7 [SIMD](https://github.com/yungyuc/nsd/blob/master/notebook/current/07_simd/simd.ipynb)

Parallelism and x86 assembly for SIMD.

1. Types of parallelism
   1. Shared-memory parallelism
   2. Distributed-memory parallelism
   3. Vector processing
2. SIMD instructions
   1. CPU capabilities
   2. x86 intrinsic functions
   3. Symbol table
   4. Inspect assembly: 1, 3, 5 multiplications

# Unit 8 [Memory management](https://github.com/yungyuc/nsd/blob/master/notebook/current/08_mem/mem.ipynb)

Numerical software tends to use as much memory as a workstation has.  The memory has two major uses: (i) to hold the required huge amount of data, and (ii) to gain speed.

1. Linux memory model: stack, heap, and memory map
2. C memory management API
3. C++ memory management API
4. STL allocator API
3. Object counter

# Unit 9 [Ownership and smart pointers](https://github.com/yungyuc/nsd/blob/master/notebook/current/09_smart/smart.ipynb)

Ownership and memory management using C++ smart pointers.

1. Pointers and ownership
    1. Raw pointer
    2. Reference
    3. Ownership
    4. Smart pointers
        1. `unique_ptr`
        2. `shared_ptr`
2. Revisit shared pointer
    1. Make Data exclusively managed by `shared_ptr`
    2. Get `shared_ptr` from `this`
    3. Cyclic reference and `weak_ptr`

# Unit 10 [Modern C++](https://github.com/yungyuc/nsd/blob/master/notebook/current/10_moderncpp/moderncpp.ipynb)

Copy elision and move semantics.  Variadic template and perfect forwarding.  Closure.

1. Copy elision / return value optimization
    1. Make a tracker class for copy construction
    2. Show the copy elision in action
    3. Inspect the assembly
2. Move semantics and copy elision
    1. Forced move is a bad idea
3. Data concatenation
    1. Style 1: return `vector`
    2. Style 2: use output `vector`
    3. Style 3: use a class for both return and output argument
4. Variadic template
5. Perfect forwarding
6. Lambda expression
    1. Keep a lambda in a local variable
    2. Difference between `auto` and `std::function`
7. Closure
    1. Comments on functional style

# Unit 11 [C++ and C for Python](https://github.com/yungyuc/nsd/blob/master/notebook/current/11_cpppy/cpppy.ipynb)

Use C++ and C to control the CPython interpreter.

1. Expectation from Python
   1. Linear wave
   2. The inviscid Burgers equation
2. Pybind11 build system
   1. Setuptools
   2. Cmake with a sub-directory
   3. Cmake with install pybind11
3. Additional wrapping layer for customization
4. Wrapping API
   1. Functions and property
   2. Named ane keyword arguments
   3. What happens in Python stays in Python (or pybind11)
5. Manipulate Python objects in C++
6. Python containers
   1. `tuple`
   2. `list`
   3. `dict`
7. Use cpython API with pybind11
   1. `PyObject` reference counting
8. Built-in types
   1. Cached value
   2. Attribute access
   3. Function call
   4. Tuple
   5. Dictionary
   6. List
9. Useful operations
    1. Import
    2. Exception
10. Python memory management
    1. PyMem interface
    2. Small memory optimization
    3. Tracemalloc

# Unit 12 [Array code in C++](https://github.com/yungyuc/nsd/blob/master/notebook/current/12_arraycode/arraycode.ipynb)

Dissect the array-based code and element-based code and when to use them.  Software architecture that take advantage of array-based code.

1. Python is slow but easy to write
2. Speed up by using numpy (still in Python)
3. Xtensor: write iterative code in C++ speed using arrays
4. Effect of house-keeping code
5. Design interface with arrays
6. Conversion between dynamic and static semantics
7. Insert profiling code

# Unit 12 [Array-oriented design](https://github.com/yungyuc/nsd/blob/master/notebook/current/12_arraydesign/arraydesign.ipynb)

Software architecture that take advantage of array-based code.

1. Design interface with arrays
2. Conversion between dynamic and static semantics
3. Insert profiling code

# Unit 13 [Advanced Python](https://github.com/yungyuc/nsd/blob/master/notebook/current/13_advpy/advpy.ipynb)

Advanced topics in Python programming.

1. Iterator
   1. List comprehension
   2. Generator
   3. Generator expression
2. Stack frame
   1. `frame` object
3. Customizing module import with `sys.meta_path`
4. Descriptor
   1. Keep data on the instance
5. Metaclass
6. Type introspection and abstract base class (abc)
   1. Method resolution order (mro)
   2. Abstract base class (abc)
   3. Abstract method

# Miscellaneous

* Readthedocs version: https://yyc.solvcon.net/en/latest/nsd/ (with cross reference).
* NYCU syllabus link: https://timetable.nycu.edu.tw/?r=main/crsoutline&Acy=109&Sem=2&CrsNo=5301
* 2020 autumn at NCTU: [notebook/20au_nctu/](https://github.com/yungyuc/nsd/blob/master/notebook/20au_nctu/index.ipynb).
* 2020 spring at NCTU: [notebook/20sp_nctu/](https://github.com/yungyuc/nsd/blob/master/notebook/20sp_nctu/index.ipynb).
* 2019 autumn at NCTU: [notebook/19au_nctu/](https://github.com/yungyuc/nsd/blob/master/notebook/19au_nctu/index.ipynb).

Command to bootstrap a server on AWS:

```bash
curl -s https://raw.githubusercontent.com/yungyuc/nsd/master/contrib/install.sh | bash -s setup_aws
```
