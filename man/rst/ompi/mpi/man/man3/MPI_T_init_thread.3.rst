.. _MPI_T_init_thread:

MPI_T_init_thread
~~~~~~~~~~~~~~~~~
====

:ref:`MPI_T_init_thread`  - Initializes the MPI Tool information interface

SYNTAX
======

C Syntax
--------

.. code:: c

   #include <mpi.h>
   int MPI_T_init_thread(int required, int *provided)

INPUT PARAMETERS
================

-  required: Desired level of thread support (integer).

OUTPUT PARAMETERS
=================

-  provided: Available level of thread support (integer).

DESCRIPTION
===========

:ref:`MPI_T_init_thread()`  initializes the MPI tool information interface.
Calls to MPI tool functions are allowed at any point in execution
(including before :ref:`MPI_Init()`  and after :ref:`MPI_Finalize()` ) as long
as :ref:`MPI_T_init_thread()`  has been called at least once and the number
of calls to :ref:`MPI_T_init_thread()`  is greater than the number of calls
to :ref:`MPI_T_finalize()` . If at any point in execution the number of
calls to :ref:`MPI_T_finalize()`  equals the number of calls to
:ref:`MPI_T_init_thread()`  the MPI tool interface will no longer be
available until another call to :ref:`MPI_T_init_thread()` .

:ref:`MPI_T_init_thread()` , like :ref:`MPI_Init_thread()` , has a provision to
request a certain level of thread support in ``required``:

-  :ref:`MPI_THREAD_SINGLE` : Only one thread will execute.
-  :ref:`MPI_THREAD_FUNNELED` : If the process is multithreaded, only the
   thread that called :ref:`MPI_Init_thread()`  will make MPI calls.
-  :ref:`MPI_THREAD_SERIALIZED` : If the process is multithreaded, only one
   thread will make MPI library calls at one time.
-  :ref:`MPI_THREAD_MULTIPLE` : If the process is multithreaded, multiple
   threads may call MPI at once with no restrictions.

The level of thread support available to the program is set in
``provided``. In Open MPI, the value is dependent on how the library was
configured and built. Note that there is no guarantee that ``provided``
will be greater than or equal to ``required``.

NOTES
=====

It is the caller's responsibility to check the value of ``provided``, as
it may be less than what was requested in ``required``.

ERRORS
======

:ref:`MPI_T_init_thread()`  will fail if:

-  :ref:`MPI_T_ERR_MEMORY` : Out of memory
-  :ref:`MPI_T_ERR_CANNOT_INIT` : Interface not in the state to be
   initialized


.. seealso:: :ref:`MPI_T` :ref:`MPI_Init_thread` :ref:`MPI_T_finalize` 
