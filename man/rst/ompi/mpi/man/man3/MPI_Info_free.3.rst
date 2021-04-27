.. _MPI_Info_free:

MPI_Info_free
~~~~~~~~~~~~~

:ref:`MPI_Info_free`  - Frees an info object.

SYNTAX
======

C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_Info_free(MPI_Info *info)

Fortran Syntax
--------------

.. code-block:: fortran
   :linenos:

   USE MPI
   ! or the older form: INCLUDE 'mpif.h'
   MPI_INFO_FREE(INFO, IERROR)
   	INTEGER		INFO, IERROR

Fortran 2008 Syntax
-------------------

.. code-block:: fortran
   :linenos:

   USE mpi_f08
   MPI_Info_free(info, ierror)
   	TYPE(MPI_Info), INTENT(INOUT) :: info
   	INTEGER, OPTIONAL, INTENT(OUT) :: ierror

INPUT/OUTPUT PARAMETER
======================

* ``info``: Info object (handle). 

OUTPUT PARAMETER
================

* ``IERROR``: Fortran only: Error status (integer). 

DESCRIPTION
===========

:ref:`MPI_Info_free`  frees *info* and sets it to :ref:`MPI_INFO_NULL` .

ERRORS
======

Almost all MPI routines return an error value; C routines as the value
of the function and Fortran routines in the last argument.

Before the error value is returned, the current MPI error handler is
called. By default, this error handler aborts the MPI job, except for
I/O function errors. The error handler may be changed with
:ref:`MPI_Comm_set_errhandler` ; the predefined error handler :ref:`MPI_ERRORS_RETURN` 
may be used to cause error values to be returned. Note that MPI does not
guarantee that an MPI program can continue past an error.

SEE ALSO
========

| :ref:`MPI_Info_create` 
| :ref:`MPI_Info_delete` 
| :ref:`MPI_Info_dup` 
| :ref:`MPI_Info_get` 
| :ref:`MPI_Info_set` 

.. seealso:: :ref:`MPI_Comm_set_errhandler` :ref:`MPI_Info_create` :ref:`MPI_Info_delete` :ref:`MPI_Info_dup` :ref:`MPI_Info_get` :ref:`MPI_Info_set`
