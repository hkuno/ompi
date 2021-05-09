.. _Open-MPI:

Open-MPI
~~~~~~~~
====

Open MPI - General information

OPEN MPI
========

Open MPI is an open source implementation of MPI (message-passing
interface), the industry-standard specification for writing
message-passing programs. Message passing is a programming model that
gives the programmer explicit control over interprocess communication.

The MPI specification was developed by the MPI Forum, a group of
software developers, computer vendors, academics, and computer-science
researchers whose goal was to develop a standard for writing
message-passing programs that would be efficient, flexible, and
portable.

The outcome, known as the MPI Standard, was first published in 1993; its
most recent version (MPI-3.1) was published in June 2015. Open MPI
includes all MPI 3.1-compliant routines.

For more information about Open MPI, see https://www.open-mpi.org.

The MPI standards are available at https://www.mpi-forum.org.

MAN PAGE SYNTAX
===============

Man pages for Open MPI and Open MPI I/O routines are named according to
C syntax, that is, they begin with the prefix ``MPI_``, all in
uppercase, and the first letter following the ``MPI_`` prefix is also
uppercase. The rest of the letters in the routine are all lowercase, for
example, :ref:`MPI_Comm_get_attr` .

ENVIRONMENT
===========

To fine-tune your Open MPI environment, you can either use arguments to
the ``mpirun`` or ``mpiexec`` commands, or you can use MCA parameters.

For more information on arguments, see the ``mpirun``\ (1) man page.

For a complete listing of MCA parameters and their descriptions, issue
the command ``ompi_info --all``. This information also appears in the
FAQ on the Open MPI web site at
https://www.open-mpi.org/faq/?category=tuning#mca-params.

ERRORS
======

All MPI routines (except :ref:`MPI_Wtime`  and :ref:`MPI_Wtick` ) return an
error value; C routines as the value of the function and Fortran
routines in the last argument. Before the value is returned, the current
MPI error handler is called. By default, this error handler aborts the
MPI job. The error handler may be changed with
:ref:`MPI_Comm_set_errhandler` ; the predefined error handler
:ref:`MPI_ERRORS_RETURN`  may be used to cause error values to be returned.
Note that MPI does not guarantee that an MPI program can continue past
an error.

For more information on Open MPI error codes, see ``mpi.h`` in the
``include`` directory.

Standard error return classes for Open MPI:

+--------------------------+-------------+--------------------------+
| Error name               | Error value | Description              |
+==========================+=============+==========================+
| :ref:`MPI_SUCCESS`               | 0           | Successful return code.  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_BUFFER`            | 1           | Invalid buffer pointer.  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_COUNT`             | 2           | Invalid count argument.  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_TYPE`              | 3           | Invalid datatype         |
|                          |             | argument.                |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_TAG`               | 4           | Invalid tag argument.    |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_COMM`              | 5           | Invalid communicator.    |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RANK`              | 6           | Invalid rank.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_REQUEST`           | 7           | Invalid :ref:`MPI_Request`       |
|                          |             | handle.                  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_ROOT`              | 8           | Invalid root.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_GROUP`             | 9           | Null group passed to     |
|                          |             | function.                |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_OP`                | 10          | Invalid operation.       |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_TOPOLOGY`          | 11          | Invalid topology.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_DIMS`              | 12          | Illegal dimension        |
|                          |             | argument.                |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_ARG`               | 13          | Invalid argument.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_UNKNOWN`           | 14          | Unknown error.           |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_TRUNCATE`          | 15          | Message truncated on     |
|                          |             | receive.                 |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_OTHER`             | 16          | Other error; use         |
|                          |             | Error_string.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_INTERN`            | 17          | Internal error code.     |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_IN_STATUS`         | 18          | Look in status for error |
|                          |             | value.                   |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_PENDING`           | 19          | Pending request.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_ACCESS`            | 20          | Permission denied.       |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_AMODE`             | 21          | Unsupported amode passed |
|                          |             | to open.                 |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_ASSERT`            | 22          | Invalid assert.          |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_BAD_FILE`          | 23          | Invalid file name (for   |
|                          |             | example, path name too   |
|                          |             | long).                   |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_BASE`              | 24          | Invalid base.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_CONVERSION`        | 25          | An error occurred in a   |
|                          |             | user-supplied            |
|                          |             | data-conversion          |
|                          |             | function.                |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_DISP`              | 26          | Invalid displacement.    |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_DUP_DATAREP`       | 27          | Conversion functions     |
|                          |             | could not be registered  |
|                          |             | because a data           |
|                          |             | representation           |
|                          |             | identifier that was      |
|                          |             | already defined was      |
|                          |             | passed to                |
|                          |             | :ref:`MPI_REGISTER_DATAREP` .    |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_FILE_EXISTS`       | 28          | File exists.             |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_FILE_IN_USE`       | 29          | File operation could not |
|                          |             | be completed, as the     |
|                          |             | file is currently open   |
|                          |             | by some process.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_FILE`              | 30          | Invalid file handle.     |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_INFO_KEY`          | 31          | Illegal info key.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_INFO_NOKEY`        | 32          | No such key.             |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_INFO_VALUE`        | 33          | Illegal info value.      |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_INFO`              | 34          | Invalid info object.     |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_IO`                | 35          | I/O error.               |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_KEYVAL`            | 36          | Illegal key value.       |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_LOCKTYPE`          | 37          | Invalid locktype.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_NAME`              | 38          | Name not found.          |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_NO_MEM`            | 39          | Memory exhausted.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_NOT_SAME`          | 40          | Collective argument not  |
|                          |             | identical on all         |
|                          |             | processes, or collective |
|                          |             | routines called in a     |
|                          |             | different order by       |
|                          |             | different processes.     |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_NO_SPACE`          | 41          | Not enough space.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_NO_SUCH_FILE`      | 42          | File (or directory) does |
|                          |             | not exist.               |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_PORT`              | 43          | Invalid port.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_PROC_ABORTED`      | 74          | Operation failed because |
|                          |             | a remote peer has        |
|                          |             | aborted.                 |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_QUOTA`             | 44          | Quota exceeded.          |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_READ_ONLY`         | 45          | Read-only file system.   |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_CONFLICT`      | 46          | Conflicting accesses to  |
|                          |             | window.                  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_SYNC`          | 47          | Erroneous RMA            |
|                          |             | synchronization.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_SERVICE`           | 48          | Invalid                  |
|                          |             | publish/unpublish.       |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_SIZE`              | 49          | Invalid size.            |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_SPAWN`             | 50          | Error spawning.          |
+--------------------------+-------------+--------------------------+
| MPI                      | 51          | Unsupported datarep      |
| _ERR_UNSUPPORTED_DATAREP |             | passed to                |
|                          |             | :ref:`MPI_File_set_view` .       |
+--------------------------+-------------+--------------------------+
| MPI_E                    | 52          | Unsupported operation,   |
| RR_UNSUPPORTED_OPERATION |             | such as seeking on a     |
|                          |             | file that supports only  |
|                          |             | sequential access.       |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_WIN`               | 53          | Invalid window.          |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_MEMORY`          | 54          | Out of memory.           |
+--------------------------+-------------+--------------------------+
| M                        | 55          | Interface not            |
| PI_T_ERR_NOT_INITIALIZED |             | initialized.             |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_CANNOT_INIT`     | 56          | Interface not in the     |
|                          |             | state to be initialized. |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_INVALID_INDEX`   | 57          | The enumeration index is |
|                          |             | invalid.                 |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_INVALID_ITEM`    | 58          | The item index queried   |
|                          |             | is out of range.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_INVALID_HANDLE`  | 59          | The handle is invalid.   |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_OUT_OF_HANDLES`  | 60          | No more handles          |
|                          |             | available.               |
+--------------------------+-------------+--------------------------+
| M                        | 61          | No more sessions         |
| PI_T_ERR_OUT_OF_SESSIONS |             | available.               |
+--------------------------+-------------+--------------------------+
| M                        | 62          | Session argument is not  |
| PI_T_ERR_INVALID_SESSION |             | a valid session.         |
+--------------------------+-------------+--------------------------+
| MP                       | 63          | Variable cannot be set   |
| I_T_ERR_CVAR_SET_NOT_NOW |             | at this moment.          |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_CVAR_SET_NEVER`  | 64          | Variable cannot be set   |
|                          |             | until end of execution.  |
+--------------------------+-------------+--------------------------+
| MPI                      | 65          | Variable cannot be       |
| _T_ERR_PVAR_NO_STARTSTOP |             | started or stopped.      |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_PVAR_NO_WRITE`   | 66          | Variable cannot be       |
|                          |             | written or reset.        |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_PVAR_NO_ATOMIC`  | 67          | Variable cannot be read  |
|                          |             | and written atomically.  |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_RANGE`         | 68          | Target memory is not     |
|                          |             | part of the window (in   |
|                          |             | the case of a window     |
|                          |             | created with             |
|                          |             | :ref:`MPI_WIN_CREATE_DYNAMIC` ,  |
|                          |             | target memory is not     |
|                          |             | attached).               |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_ATTACH`        | 69          | Memory cannot be         |
|                          |             | attached (e.g., because  |
|                          |             | of resource exhaustion). |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_FLAVOR`        | 70          | Passed window has the    |
|                          |             | wrong flavor for the     |
|                          |             | called function.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_RMA_SHARED`        | 71          | Memory cannot be shared  |
|                          |             | (e.g., some process in   |
|                          |             | the group of the         |
|                          |             | specified communicator   |
|                          |             | cannot expose shared     |
|                          |             | memory).                 |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_INVALID`         | 72          | Invalid use of the       |
|                          |             | interface or bad         |
|                          |             | parameter values(s).     |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_T_ERR_INVALID_NAME`    | 73          | The variable or category |
|                          |             | name is invalid.         |
+--------------------------+-------------+--------------------------+
| :ref:`MPI_ERR_LASTCODE`          | 93          | Last error code.         |
+--------------------------+-------------+--------------------------+


.. seealso:: :ref:`MPI_T` 
