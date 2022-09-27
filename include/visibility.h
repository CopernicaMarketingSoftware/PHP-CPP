/**
 *  visibility.h
 *
 *  This file defines macros used to define whether a symbol
 *  should be exported. It is only used for classes and
 *  functions that are defined in the public API to reduce
 *  the size of the symbol table, make linking and loading
 *  of the PHP-CPP library faster and generate more
 *  optimized code as a result.
 *
 *  @copyright 2015 Copernica B.V.
 */

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_PHPCPP
        #ifdef __GNUC__
            #define PHPCPP_EXPORT __attribute__ ((dllexport))
        #else
            // building PHP-CPP as dll, BUILDING_PHPCPP defined in PHP-CPP project
            #define PHPCPP_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #else
        #ifdef __GNUC__
            #define DLL_EXPORT __attribute__ ((dllimport))
            #define PHPCPP_EXPORT __attribute__ ((dllimport))
        #else
            #define DLL_EXPORT __declspec(dllimport)    // Note: actually gcc seems to also supports this syntax.

            #ifdef USING_PHPCPP_DLL
                // PHP-CPP was built as dll, USING_PHPCPP_DLL defined in extension project
                // extension uses phpcpp as shared(dll) library
                #define PHPCPP_EXPORT __declspec(dllimport)
            #else
                // PHP-CPP was built as static library, extension uses phpcpp as static library
                #define PHPCPP_EXPORT
            #endif
        #endif
    #endif

    // MODULE_EXPORT defined in extension source
    #ifdef __GNUC__
        #define MODULE_EXPORT __attribute__ ((dllexport))
    #else
        #define MODULE_EXPORT __declspec(dllexport)
    #endif

#else
    #define MODULE_EXPORT __attribute__ ((visibility ("default")))
    #define PHPCPP_EXPORT __attribute__ ((visibility ("default")))
#endif
