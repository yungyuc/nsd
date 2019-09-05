# Numerical Software Development

Copyright 2019, Yung-Yu Chen <yyc@solvcon.net>.  All rights reserved.
[![Build Status](https://travis-ci.com/yungyuc/nsd.svg?branch=master)](https://travis-ci.com/yungyuc/nsd)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/yungyuc/nsd/master)

**Objectives**: This course discusses the art to build numerical software, i.e.,
computer programs applying numerical methods for solving mathematical or
physical problems.  We will be using the combination of Python and C++ and
related tools (e.g., [bash](https://www.gnu.org/software/bash/),
[git](https://git-scm.com), [make](https://www.gnu.org/software/make/), etc.)
to learn the modern development processes.  By completing this course, students
will acquire the fundamental skills for developing modern numerical software.

**Prerequisites**: This is a graduate or senior level course open to students
who have taken engineering mathematics or equivalence.  Working knowledge of
Linux and Unix-like is required.  Prior knowledge to numerical methods is
recommended.  The instructor uses English in the lectures and discussions.

# How to study

* This is a practical course.  No textbook is available for this specific
  interdisciplinary subject.
* To study the subject, students are required to research with online documents
  and source code, and write programs to practice.
* In-class instruction and [course notes](https://github.com/yungyuc/nsd) are
  provided for guidance.
* References:
  * Computer Systems: A Programmer's Perspective, Randal E. Bryant and David R. O'Hallaron: https://csapp.cs.cmu.edu/
  * Python documentation: https://docs.python.org/3/
  * Cppreference: https://en.cppreference.com/
  * [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/),
     Scott Meyer, O'Reilly, 2014
  * Source code: [cpython](https://github.com/python/cpython),
    [numpy](https://github.com/numpy/numpy),
    [xtensor](https://github.com/QuantStack/xtensor), and
    [pybind11](https://github.com/pybind/pybind11)

# Course design

* There are 15 or more lectures for the subjects of numerical software
  developing using Python and C++.
* There is usually homework given after a lecture to exercise the lectured
  materials, and it usually requires students to write computer programs in
  Python and/or C++.
* Mid-term examination will be conducted to assess students' understandings
  to the analytical materials.
* Term project will be used to assess students' overall coding skills.
  Presentation is required.  Failure to present results in **0 point** for this
  part.
* Grading: homework 30%, mid-term exam: 30%, term project: 40%.
* Lectures:
  1. Python and numpy
  2. C++ and computer architecture
  3. Fundamental engineering practices
  4. Memory management
  5. Matrix operations
  6. Cache optimization
  7. SIMD
  8. Modern C++ I: ownership and meta-programming
  9. Modern C++ II: more than templates
  10. xtensor: arrays in C++
  11. pybind11: binding between Python and C++
  12. cpython API: operate Python from C
  13. Profiling
  14. Array-oriented design
  15. Advanced Python
  16. (Optional) useful architectures for hybrid code
  17. (Optional) application embedding Python
