/**
 *  deprecated.h
 *
 *  This file defines macros to mark a function as deprecated
 *  in a way that works across different compilers.
 *
 *  It is used for functions that still work, but should no
 *  longer be used because they may be removed in an upcoming
 *  version of PHP-CPP
 *
 *  @copyright 2016 Copernica B.V.
 */

#if __cplusplus >= 201402L
    #define DEPRECATED [[deprecated]]
#elif defined __GNUC__
    #define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define DEPRECATED __declspec(deprecated)
#endif
