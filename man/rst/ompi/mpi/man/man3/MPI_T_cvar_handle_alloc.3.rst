NAME
----

``MPI_T_cvar_handle_alloc``, ``MPI_T_cvar_handle_free`` - Allocate/free
contol variable handles

SYNTAX
------

C Syntax
~~~~~~~~

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_cvar_handle_alloc(int cvar_index, void *obj_handle,
                               MPI_T_cvar_handle *handle, int *count)

   int MPI_T_cvar_handle_free(MPI_T_cvar_handle *handle)

DESCRIPTION
-----------

``MPI_T_cvar_handle_alloc`` binds the control variable specified in
``*cvar``_index* to the MPI object specified in ``*obj``_handle*. If
``MPI_T_cvar_get_info`` returns ``MPI_T_BIND_NO_OBJECT`` as the binding of the
variable the ``*obj``_handle* argument is ignored. The number of values
represented by this control variable is returned in the ``*count``*
parameter. If the control variable represents a string then ``*count``* will
be the maximum length of the string.

``MPI_T_cvar_handle_free`` frees a handle allocated by
``MPI_T_cvar_handle_alloc`` and sets the ``*handle``* argument to
``MPI_T_CVAR_HANDLE_NULL``.

NOTES
-----

Open MPI does not currently support binding MPI objects to control
variables so the ``*obj``_handle* argument is always ignored.

ERRORS
------

``MPI_T_cvar_handle_alloc``() will fail if:

[``MPI_T_ERR_NOT_INITIALIZED``]
   The MPI Tools interface not initialized

[``MPI_T_ERR_INVALID_INDEX``]
   The control variable index is invalid

[``MPI_T_ERR_OUT_OF_HANDLES``]
   No more handles available

``MPI_T_cvar_handle_free``() will fail if:

[``MPI_T_ERR_NOT_INITIALIZED``]
   The MPI Tools interface not initialized

[``MPI_T_ERR_INVALID_HANDLE``]
   The handle is invalid

SEE ALSO
--------

.. code-block:: c
   :linenos:

   MPI_T_cvar_get_info
