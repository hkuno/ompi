NAME
----

``MPI_Op_commutative`` - Query of commutativity of reduction operation.

SYNTAX
------

C Syntax
~~~~~~~~

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_Op_commutative(MPI_Op op, int *commute)

Fortran Syntax
~~~~~~~~~~~~~~

.. code-block:: fortran
   :linenos:

   USE MPI
   ! or the older form: INCLUDE 'mpif.h'
   MPI_OP_COMMUTATIVE(OP, COMMUTE, IERROR)
   	LOGICAL	COMMUTE
   	INTEGER	OP, IERROR

Fortran 2008 Syntax
~~~~~~~~~~~~~~~~~~~

.. code-block:: fortran
   :linenos:

   USE mpi_f08
   MPI_Op_commutative(op, commute, ierror)
   	TYPE(MPI_Op), INTENT(IN) :: op
   	INTEGER, INTENT(OUT) :: commute
   	INTEGER, OPTIONAL, INTENT(OUT) :: ierror

INPUT PARAMETER
---------------


OUTPUT PARAMETERS
-----------------


* ``Fortran only``: 

DESCRIPTION
-----------

Reduction operations can be queried for their commutativity.

ERRORS
------

Almost all MPI routines return an error value; C routines as the value
of the function and Fortran routines in the last argument.

Before the error value is returned, the current MPI error handler is
called. By default, this error handler aborts the MPI job, except for
I/O function errors. The error handler may be changed with
``MPI_Comm_set_errhandler``; the predefined error handler ``MPI_ERRORS_RETURN``
may be used to cause error values to be returned. Note that MPI does not
guarantee that an MPI program can continue past an error.

SEE ALSO
--------

``MPI_Op_create``
