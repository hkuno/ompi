.. _MPI_T_category_get_num:

MPI_T_category_get_num
~~~~~~~~~~~~~~~~~~~~~~

:ref:`MPI_T_category_get_num`  - Query the number of categories

SYNTAX
======

C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_category_get_num(int *num_cat)

OUTPUT PARAMETERS
=================

* ``num_cat``: Current number of categories 

DESCRIPTION
===========

:ref:`MPI_T_category_get_num`  can be used to query the current number of
categories.

ERRORS
======

:ref:`MPI_T_category_get_num()`  will fail if:

[:ref:`MPI_T_ERR_NOT_INITIALIZED]` 
   The MPI Tools interface not initialized
