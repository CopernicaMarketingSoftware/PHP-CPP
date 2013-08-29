/**
 *  phpcpp.h
 *
 *  Library to build PHP extensions with CPP
 * 
 *  @copyright 2013 CopernicA BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

/**
 *  Other C and C++ libraries that PhpCpp depends on
 */
#include <string.h>
#include <string>
#include <initializer_list>
#include <vector>

/**
 *  Include all headers files that are related to this library
 */
#include <phpcpp/type.h>
#include <phpcpp/request.h>
#include <phpcpp/argument.h>
#include <phpcpp/value.h>
#include <phpcpp/member.h>
#include <phpcpp/arguments.h>
#include <phpcpp/function.h>
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

