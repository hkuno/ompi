NAME
----

``MPI_T_category_get_num`` - Query the number of categories

SYNTAX
------

C Syntax
~~~~~~~~

.. code-block:: c
   :linenos:

   #include <mpi.h>
   int MPI_T_category_get_num(int *num_cat)

OUTPUT PARAMETERS
-----------------


DESCRIPTION
-----------

``MPI_T_category_get_num`` can be used to query the current number of
categories.

ERRORS
------

``MPI_T_category_get_num()`` will fail if:

[``MPI_T_ERR_NOT_INITIALIZED]``
   The MPI Tools interface not initialized
