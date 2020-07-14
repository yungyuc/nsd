==================================================
Proposal template for spatial search (spacesearch)
==================================================

Basic information
=================

The GitHub repository is at: (URL)

Problem to solve
================

Operations on geometric entities (points, line segments, etc.) need to know the
geometric relation among them, e.g., the nearest entities, entities with in a
range, etc.  The queries can be done by looping through all the entities of
interest.  But oftentimes we need to make the queries frequently, and the
runtime of linear search becomes a concern.  To reduce the time complexity, a
class of data structures called spatial index is developed [1]_.

This project is to implement a library hosting a spatial index or spatial
indices to perform flexible spatial queries with high speed.

Prospective users
=================

Applications that need to query geometry entities can use this library to speed
up searches in space.

System architecture
===================

List the set of geometric entities supported by the library and their data
structure.

Describe the index data structure, the algorithms for maintaining the data
structure, and the queries to be supported.

An object ``index`` is created to house the data for the spatial index.
Support of extensible queries surround the index object.

Describe how the memory management is done for the search engine.

API description
===============

Show how your system can be programmed.  You are supposed to implement the
system using both C++ and Python.  Describe how a user writes a script in the
system.

List the API for the index data housekeeping and query.  Provide example code
if possible.

Engineering infrastructure
==========================

The system uses C++ to define the data structure and Python for high-level API,
and will be built with `cmake <https://cmake.org>`__.

A profiling system should be provided.  If possible, include it in automatic
testing.

The repository is hosted on GitHub and will use GitHub Action for continuous
integration.

Schedule
========

The development of the project will take 8 weeks.  Survey of the spatial
indices to be employed should be done before the start of the development.

* Week 1: Define the geometry entities to be supported by the search engine.
  Design the data structure the house them with prototype code implemented.
* Week 2: Implement the spatial index that supports the geometry entities
  defined.  One or more to-be-supported queries should be determined.
* Week 3: Implement the planned queries (1/3) along with automatic testing.
* Week 4: Continue to implement queries (2/3).  Start writing the documents
  (presentation).
* Week 5: Continue to implement queries (3/3).  Supplement automatic testing
  for the index data structure and its housekeeping code.
* Week 6: Revise the system design as well as the implementation.
* Week 7: Finalize the API.  Final refactor.
* Week 8: Automate release process and include final presentation.

References
==========

.. [1]
   https://www.boost.org/doc/libs/1_73_0/libs/geometry/doc/html/geometry/spatial_indexes/introduction.html
