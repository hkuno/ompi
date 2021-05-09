.. _MPI_T_category_get_categories:

MPI_T_category_get_categories
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:ref:`MPI_T_category_get_categories`  - Query which categories are in a
category

SYNTAX
======

C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_category_get_categories(int cat_index, int len, int indices[])

INPUT PARAMETERS
================

* ``cat_index``: Index of the category to be queried. 

* ``len``: The length of the indices array. 

OUTPUT PARAMETERS
=================

* ``indices``: An integer array of size len, indicating category indices. 

DESCRIPTION
===========

:ref:`MPI_T_category_get_categories`  can be used to query which other
categories are in a category.

ERRORS
======

:ref:`MPI_T_category_get_categories()`  will fail if:

[:ref:`MPI_T_ERR_NOT_INITIALIZED]` 
   The MPI Tools interface not initialized

[:ref:`MPI_T_ERR_INVALID_INDEX]` 
   The category index is invalid
