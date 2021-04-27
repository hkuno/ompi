.. _MPI_T_category_get_pvars:

MPI_T_category_get_pvars
~~~~~~~~~~~~~~~~~~~~~~~~

:ref:`MPI_T_category_get_pvars`  - Query which performance variables are in
a category

SYNTAX
======

C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_category_get_pvars(int cat_index, int len, int indices[])

INPUT PARAMETERS
================

* ``cat_index``: Index of the category to be queried. 

* ``len``: The length of the indices array. 

OUTPUT PARAMETERS
=================

* ``indices``: An integer array of size len, indicating performance variable indices. 

DESCRIPTION
===========

:ref:`MPI_T_category_get_pvars`  can be used to query which performance
variables are contained in a particular category. A category contains
zero or more performance variables.

ERRORS
======

:ref:`MPI_T_category_get_pvars()`  will fail if:

[:ref:`MPI_T_ERR_NOT_INITIALIZED]` 
   The MPI Tools interface not initialized

[:ref:`MPI_T_ERR_INVALID_INDEX]` 
   The category index is invalid

.. seealso:: :ref:`MPI_T_category_get_pvars()`
