NAME
----

``MPI_Type_hindexed`` - Creates an indexed datatype with offsets in
bytes -- use of this routine is deprecated.

SYNTAX
------

C Syntax
~~~~~~~~

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_Type_hindexed(int count, int *array_of_blocklengths,
   	MPI_Aint *array_of_displacements, MPI_Datatype oldtype,
   	MPI_Datatype *newtype)

Fortran Syntax
~~~~~~~~~~~~~~

.. code-block:: fortran
   :linenos:

   INCLUDE 'mpif.h'
   MPI_TYPE_HINDEXED(COUNT, ARRAY_OF_BLOCKLENGTHS,
   		ARRAY_OF_DISPLACEMENTS, OLDTYPE, NEWTYPE, IERROR)
   	INTEGER	COUNT, ARRAY_OF_BLOCKLENGTHS(*)
   	INTEGER	ARRAY_OF_DISPLACEMENTS(*), OLDTYPE, NEWTYPE
   	INTEGER	IERROR

INPUT PARAMETERS
----------------



