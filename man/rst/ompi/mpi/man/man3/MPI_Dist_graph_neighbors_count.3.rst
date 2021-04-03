NAME
----

``MPI_Dist_graph_neighbors_count`` - Returns the number of in and out
edges for the calling processes in a distributed graph topology and a
flag indicating whether the distributed graph is weighted.

SYNTAX
------

C Syntax
~~~~~~~~

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_Dist_graph_neighbors_count(MPI_Comm comm, int *indegree,
   	int *outdegree, int *weighted)

Fortran Syntax
~~~~~~~~~~~~~~

.. code-block:: fortran
   :linenos:

   USE MPI
   ! or the older form: INCLUDE 'mpif.h'
   MPI_DIST_GRAPH_NEIGHBORS_COUNT(COMM, INDEGREE, OUTDEGREE, WEIGHTED, IERROR)
   	INTEGER	COMM, INDEGREE, OUTDEGREE, IERROR
           LOGICAL WEIGHTED

Fortran 2008 Syntax
~~~~~~~~~~~~~~~~~~~

.. code-block:: fortran
   :linenos:

   USE mpi_f08
   MPI_Dist_graph_neighbors_count(comm, indegree, outdegree, weighted, ierror)
   	TYPE(MPI_Comm), INTENT(IN) :: comm
   	INTEGER, INTENT(IN) :: indegree, outdegree
   	INTEGER, INTENT(OUT) :: weighted
   	INTEGER, OPTIONAL, INTENT(OUT) :: ierror

INPUT PARAMETERS
----------------


OUTPUT PARAMETERS
-----------------




* ``Fortran only``: 

DESCRIPTION
-----------

``MPI_Dist_graph_neighbors_count`` and ``MPI_Graph_neighbors`` provide adjacency
information for a distributed graph topology.
``MPI_Dist_graph_neighbors_count`` returns the number of sources and
destinations for the calling process.

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

``MPI_Dist_graph_neighbors``
