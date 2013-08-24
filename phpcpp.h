/**
 *  phpcpp.h
 *
 *  Library to build PHP extensions with CPP
 * 
 *  @copyright 2013 CopernicA BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

/**
 *  Include all headers files that are related to this library
 */
#include <phpcpp/extension.h>

/**
 *  Macro to export a function
 */
#if defined(__GNUC__) && __GNUC__ >= 4
#	define PHPCPP_EXPORT __attribute__ ((visibility("default")))
#else
#	define PHPCPP_EXPORT
#endif

/**
 *  Macro to activate the extension
 */
#define PHP_CPP_EXTENSION(classname)    extern "C" { PHPCPP_EXPORT void *get_module() { static classname extension; return extension.entry(); } }

