===============================================================
Proposal template for unstructured mesh partitioning (unstpart)
===============================================================

Basic information
=================

The GitHub repository is at: (URL)

Problem to solve
================

The fields of numerical solution of partial differential equations (PDEs)
depend on accurate discretization of space.  The entities describing the
discretized space is called mesh or grid.  Although meshless numerical methods
have been developed, mesh-based methods are used in industrial applications
[1]_.

The mesh can be broadly categorized into structured and unstructured mesh.  The
structured mesh define the connectivities between the elements of the
discretized space using the information intrinsic to the dimensionality, while
the unstructured mesh allow free definition.  The connectivity determines the
shapes available to the elements of the mesh.  For example, in two-dimensional
space, the mesh elements in a structured mesh need to be quadrilateral.  But
they can be of any shape, e.g., a simplex (triangle), quadrilateral, or a
convex polygon, in an unstructured mesh.

One of the plausible features of unstructured mesh is to enable flexible
distribution of data for parallel computing.  The connectivity between elements
may form a graph.  By turning the mesh into a graph-partitioning problem, the
mesh can be decomposed into sub-mesh.  By controlling the amount of
interconnection, the communication between sub-mesh can be minimized.

This project will implement a library that takes the unstructured mesh and
partitions it into sub-mesh, along with necessary input, output, and data
conversion.

Prospective users
=================

PDE solvers that utilize unstructured mesh will use this library to prepare for
the data for distributed parallel computing.

System architecture
===================

The system includes the following components:

1. A ``mesh`` object for the unstructured mesh.
2. A ``graph`` object for the connectivity of the mesh.

The ``mesh`` and ``graph`` objects interact with each other and provide
necessary methods.

API description
===============

Show how your system can be programmed.  You are supposed to implement the
system using both C++ and Python.  Describe how a user writes a script in the
system.

Engineering infrastructure
==========================

The system uses C++ to define the data structure and Python for high-level API,
and will be built with `cmake <https://cmake.org>`__.

The repository is hosted on GitHub and will use GitHub Action for continuous
integration.

Schedule
========

The development of the project will take 8 weeks:

* Week 1: Define the geometry of the unstructured mesh using the first
  prototype for the data structure.  The code may use C++ and/or Python (it is
  not yet known at the moment).  The dimensionality of the mesh needs to be
  determined.
* Week 2: Implement a simple viewer.  The purpose is for manual demonstration,
  debugging, and trouble-shooting.
* Week 3: Add unit tests for the code already developed or prototyped for the
  objects related to ``mesh`` and run them automatically in GitHub Action.
  Input, output, and data conversion may be needed.
* Week 4: Conversion from ``mesh`` to ``graph``, the data structures, and unit
  tests.
* Week 5&6: Implement and test the graph partitioning algorithms.  Start
  writing the documents (presentation).
* Week 7: Finalize the API for the objects related to ``mesh`` and ``graph``.
* Week 8: Automate release process and include final presentation.

References
==========

.. [1] D. J. Mavriplis, "Unstructured Grid Techniques," Annual Review of Fluid
   Mechanics, vol. 29, Jan. 1997, doi: 10.1146/annurev.fluid.29.1.473.
