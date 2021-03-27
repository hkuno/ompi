NAME
----

``*shmem``_char_g*\ (3), ``*shmem``_float_g*\ (3), ``*shmem``_int_g*\ (3),
``*shmem``_long_g*\ (3), ``*shmem``_short_g*\ (3), ``*shmem``_longlong_g*\ (3),
``*shmem``_longdouble_g*\ (3) - These routines provide a low latency
mechanism to read basic types (char, short, int, float, double, long,
long long, long double) from symmetric data objects on remote PEs.

SYNOPSIS
--------

C or C++:

.. code-block:: FOOBAR_ERROR
   :linenos:

   #include <mpp/shmem.h>


   char shmem_char_g(const char *addr, int pe);

   short shmem_short_g(const short *addr, int pe);

   int shmem_int_g(const int *addr, int pe);

   long shmem_long_g(const long *addr, int pe);

   long shmem_longlong_g(const long long *addr, int pe);

   float shmem_float_g(const float *addr, int pe);

   double shmem_double_g(const double *addr, int pe);

   long shmem_longdouble_g(const long double *addr, int pe);

DESCRIPTION
-----------

These routines provide a very low latency get capability for single
elements of most basic types.

The arguments are as follows:

addr
   The remotely accessible array element or scalar data object which
   will receive the data on the remote PE.

pe
   The number of the remote PE.

SEE ALSO
--------

*intro_shmem*\ (3), *shmem_get*\ (3)
