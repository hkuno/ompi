NAME
----

``*shmem``_character_put*\ (3), ``*shmem``_complex_put*\ (3),
``*shmem``_double_put*\ (3), ``*shmem``_float_put*\ (3), ``*shmem``_int_put*\ (3),
``*shmem``_integer_put*\ (3), ``*shmem``_logical_put*\ (3),
``*shmem``_long_put*\ (3), ``*shmem``_longdouble_put*\ (3),
``*shmem``_longlong_put*\ (3), ``*shmem``_put4*\ (3), ``*shmem``_put8*\ (3),
``*shmem``_put32*\ (3), ``*shmem``_put64*\ (3), ``*shmem``_put128*\ (3),
``*shmem``_putmem*\ (3), ``*shmem``_real_put*\ (3), ``*shmem``_short_put*\ (3) -
Transfers data to a specified processing element (PE)

SYNOPSIS
--------

C or C++:
.. code-block:: FOOBAR_ERROR
   :linenos:

   #include <mpp/shmem.h>

   void shmem_double_put(double *target, const double *source,
     size_t len, int pe);

   void shmem_float_put(float *target, const float *source,
     size_t len, int pe);

   void shmem_int_put(int *target, const int *source, size_t len,
     int pe);

   void shmem_long_put(long *target, const long *source,
     size_t len, int pe);

   void shmem_longdouble_put(long double *target,
     const long double *source, size_t len, int pe);

   void shmem_longlong_put(long long *target,
     const long long *source, size_t len, int pe);

   void shmem_put32(void *target, const void *source, size_t len,
     int pe);

   void shmem_put64(void *target, const void *source, size_t len,
     int pe);

   void shmem_put128(void *target, const void *source, size_t len,
     int pe);

   void shmem_putmem(void *target, const void *source, size_t len,
     int pe);

   void shmem_short_put(short *target, const short *source,
     size_t len, int pe);

Fortran:
.. code-block:: FOOBAR_ERROR
   :linenos:

   INCLUDE "mpp/shmem.fh"

   INTEGER len, pe

   CALL SHMEM_CHARACTER_PUT(target, source, len, pe)

   CALL SHMEM_COMPLEX_PUT(target, source, len, pe)

   CALL SHMEM_DOUBLE_PUT(target, source, len, pe)

   CALL SHMEM_INTEGER_PUT(target, source, len, pe)

   CALL SHMEM_LOGICAL_PUT(target, source, len, pe)

   CALL SHMEM_PUT(target, source, len, pe)

   CALL SHMEM_PUT4(target, source, len, pe)

   CALL SHMEM_PUT8(target, source, len, pe)

   CALL SHMEM_PUT32(target, source, len, pe)

   CALL SHMEM_PUT64(target, source, len, pe)

   CALL SHMEM_PUT128(target, source, len, pe)

   CALL SHMEM_PUTMEM(target, source, len, pe)

   CALL SHMEM_REAL_PUT(target, source, len, pe)

DESCRIPTION
-----------

These routines transfer *``*nelems``** elements of the data object at
address *``*source``** on the calling PE, to the data object at address
*``*target``** on the remote PE *``*pe``**. These routines start the remote
transfer and may return before the data is delivered to the remote PE.

The delivery of data into the data object on the destination PE from
different put calls may occur in any order. Because of this, two
successive put operations may deliver data out of order unless a call to
``*shmem``_fence*\ (3) is introduced between the two calls.

The arguments are as follows:

target
   Data object to be updated on the remote PE. This data object must be
   remotely accessible.

source
   Data object containing the data to be copied.

len
   Number of elements in the target and source arrays. len must be of
   type integer. If you are using Fortran, it must be a constant,
   variable, or array element of default integer type.

pe
   PE number of the remote PE. pe must be of type integer. If you are
   using Fortran, it must be a constant, variable, or array element of
   default integer type.

The target and source data objects must conform to certain typing
constraints, which are as follows:

*``*shmem``_putmem**: Fortran: Any noncharacter type. C: Any data type. len is scaled in
   bytes.

*``*shmem``_put4, shmem_put32:** Any noncharacter type that has a storage size
   equal to 32 bits.

*``*shmem``_put8, shmem_put64:** Any noncharacter type that has a storage size
   equal to 64 bits.

*``*shmem``_put128:** Any noncharacter type that has a storage size equal to 128
   bits.

*``*shmem``_short_put:** Elements of type short.

*``*shmem``_int_put:** Elements of type int.

*``*shmem``_long_put:** Elements of type long.

*``*shmem``_longlong_put:** Elements of type long long.

*``*shmem``_float_put:** Elements of type float.

*``*shmem``_double_put:** Elements of type double.

*``*shmem``_longdouble_put:** Elements of type long double.

**SHMEM_CHARACTER_PUT:** Elements of type character. len is the number of
   characters to transfer. The actual character lengths of the source
   and target variables are ignored.

**SHMEM_COMPLEX_PUT:** Elements of type complex of default size.

**SHMEM_DOUBLE_PUT:** (Fortran) Elements of type double precision.

**SHMEM_INTEGER_PUT:** Elements of type integer.

**SHMEM_LOGICAL_PUT:** Elements of type logical.

**SHMEM_REAL_PUT:** Elements of type real.
   If you are using Fortran, data types must be of default size. For
   example, a real variable must be declared as REAL, REAL*4, or
   REAL(KIND=4).

EXAMPLES
--------

The following shmem_put example is for C/C++ programs:
.. code-block:: FOOBAR_ERROR
   :linenos:

   #include <stdio.h>
   #include <mpp/shmem.h>

   main()
   {
     long source[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
     static long target[10];
     shmem_init();

     if (shmem_my_pe() == 0) {
       /* put 10 words into target on PE 1 */
       shmem_long_put(target, source, 10, 1);
     }
     shmem_barrier_all();  /* sync sender and receiver */
     if (shmem_my_pe() == 1)
       shmem_udcflush();  /* not required on Altix systems */
     printf("target[0] on PE %d is %d\n", shmem_my_pe(), target[0]);
   }

SEE ALSO
--------

*intro_shmem*\ (3), *shmem_iput*\ (3), *shmem_quiet*\ (3)
