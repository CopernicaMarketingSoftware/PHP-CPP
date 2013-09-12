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
#include <map>
#include <memory>
#include <set>

/**
 *  Include all headers files that are related to this library
 */
#include <phpcpp/hiddenpointer.h>
#include <phpcpp/type.h>
#include <phpcpp/environment.h>
#include <phpcpp/argument.h>
#include <phpcpp/byval.h>
#include <phpcpp/byref.h>
#include <phpcpp/value.h>
#include <phpcpp/global.h>
#include <phpcpp/member.h>
#include <phpcpp/parameters.h>
#include <phpcpp/function.h>
#include <phpcpp/extension.h>

/**
 *  Macro to export a function
 */
#if defined(__GNUC__) && __GNUC__ >= 4
#   define PHPCPP_EXPORT __attribute__ ((visibility("default")))
#else
#   define PHPCPP_EXPORT
#endif

