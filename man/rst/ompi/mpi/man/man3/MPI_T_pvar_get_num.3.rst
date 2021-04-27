.. _MPI_T_pvar_get_num:

MPI_T_pvar_get_num
~~~~~~~~~~~~~~~~~~

:ref:`MPI_T_pvar_get_num`  - Query the number of performance variables

SYNTAX
======

C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_pvar_get_num(int *num_pvar)

OUTPUT PARAMETERS
=================

* ``num_pvar``: Current number of performance variables. 

DESCRIPTION
===========

:ref:`MPI_T_pvar_get_num`  can be used to query the current number of
performance variables. The number of performance variables may increase
throughout the exection of the process but will never decrease.

ERRORS
======

:ref:`MPI_T_pvar_get_num()`  will fail if:

[:ref:`MPI_T_ERR_NOT_INITIALIZED]` 
   The MPI Tools interface not initialized

.. seealso:: :ref:`MPI_T_pvar_get_num()`
