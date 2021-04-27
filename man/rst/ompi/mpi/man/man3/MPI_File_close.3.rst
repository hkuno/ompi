.. _MPI_File_close:

MPI_File_close
~~~~~~~~~~~~~~

:ref:`MPI_File_close`  - Closes a file (collective).

SYNTAX
======


C Syntax
--------

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_File_close(MPI_File *fh)

Fortran Syntax
--------------

.. code-block:: fortran
   :linenos:

   USE MPI
   ! or the older form: INCLUDE 'mpif.h'
   MPI_FILE_CLOSE(FH, IERROR)
   	INTEGER	FH, IERROR

Fortran 2008 Syntax
-------------------

.. code-block:: fortran
   :linenos:

   USE mpi_f08
   MPI_File_close(fh, ierror)
   	TYPE(MPI_File), INTENT(INOUT) :: fh
   	INTEGER, OPTIONAL, INTENT(OUT) :: ierror

INPUT/OUTPUT PARAMETER
======================

* ``fh``: File handle (handle). 

OUTPUT PARAMETER
================

* ``IERROR``: Fortran only: Error status (integer). 

DESCRIPTION
===========

:ref:`MPI_File_close`  first synchronizes file state, then closes the file
associated with *fh.* :ref:`MPI_File_close`  is a collective routine. The user
is responsible for ensuring that all outstanding requests associated
with *fh* have completed before calling :ref:`MPI_File_close` .

ERRORS
======

Almost all MPI routines return an error value; C routines as the value
of the function and Fortran routines in the last argument.

Before the error value is returned, the current MPI error handler is
called. For MPI I/O function errors, the default error handler is set to
:ref:`MPI_ERRORS_RETURN` . The error handler may be changed with
:ref:`MPI_File_set_errhandler` ; the predefined error handler
:ref:`MPI_ERRORS_ARE_FATAL`  may be used to make I/O errors fatal. Note that MPI
does not guarantee that an MPI program can continue past an error.

.. seealso:: :ref:`MPI_File_set_errhandler`
