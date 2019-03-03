/**
 *  Includes.h
 *
 *  Startup include file to compile the phpcpp library
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Include standard C and C++ libraries
 */
#include <stdlib.h>
#include <string>
#include <initializer_list>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <list>
#include <exception>
#include <type_traits>
#include <functional>

// for debug
#include <iostream>

//#define ZTS
//#define THREAD_T pthread_t
//#define MUTEX_T pthread_mutex_t *

/**
 *  PHP includes
 */
#include <php.h>
#include <zend_exceptions.h>
#include <zend_interfaces.h>
#include <zend_ini.h>
#include <zend_closures.h>
#include <SAPI.h>

/**
 *  We don't work with older versions of PHP
 */
#if PHP_VERSION_ID < 70000
#   error "This library requires PHP version 7.0 or higher. Use PHP-CPP-LEGACY for older versions."
#endif

/**
 *  Macro to convert results to success status
 */
#define BOOL2SUCCESS(b) ((b) ? SUCCESS : FAILURE)

/**
 *  Include other files from this library
 */
#include "../include/phpcpp/visibility.h"
#include "../include/phpcpp/deprecated.h"
#include "../include/phpcpp/noexcept.h"
#include "../include/phpcpp/thread_local.h"
#include "../include/phpcpp/platform.h"
#include "../include/phpcpp/version.h"
#include "../include/phpcpp/inivalue.h"
#include "../include/phpcpp/ini.h"
#include "../include/phpcpp/exception.h"
#include "../include/phpcpp/fatalerror.h"
#include "../include/phpcpp/streams.h"
#include "../include/phpcpp/type.h"
#include "../include/phpcpp/errors.h"
#include "../include/phpcpp/hashparent.h"
#include "../include/phpcpp/value.h"
#include "../include/phpcpp/valueiterator.h"
#include "../include/phpcpp/array.h"
#include "../include/phpcpp/object.h"
#include "../include/phpcpp/globals.h"
#include "../include/phpcpp/argument.h"
#include "../include/phpcpp/byval.h"
#include "../include/phpcpp/byref.h"
#include "../include/phpcpp/global.h"
#include "../include/phpcpp/hashmember.h"
#include "../include/phpcpp/super.h"
#include "../include/phpcpp/parameters.h"
#include "../include/phpcpp/modifiers.h"
#include "../include/phpcpp/base.h"
#include "../include/phpcpp/countable.h"
#include "../include/phpcpp/arrayaccess.h"
#include "../include/phpcpp/serializable.h"
#include "../include/phpcpp/iterator.h"
#include "../include/phpcpp/traversable.h"
#include "../include/phpcpp/classtype.h"
#include "../include/phpcpp/classbase.h"
#include "../include/phpcpp/interface.h"
#include "../include/phpcpp/constant.h"
#include "../include/phpcpp/zendcallable.h"
#include "../include/phpcpp/class.h"
#include "../include/phpcpp/namespace.h"
#include "../include/phpcpp/extension.h"
#include "../include/phpcpp/call.h"
#include "../include/phpcpp/script.h"
#include "../include/phpcpp/file.h"
#include "../include/phpcpp/function.h"

/**
 *  Common header files for internal use only
 */
#include "../common/extensionbase.h"
#include "../common/streambuf.h"

/**
 *  Specific zend implementation  files for internal use only
 */
#include "init.h"
#include "callable.h"
#include "nativefunction.h"
#include "method.h"
#include "member.h"
#include "nullmember.h"
#include "numericmember.h"
#include "boolmember.h"
#include "stringmember.h"
#include "floatmember.h"
#include "arithmetic.h"
#include "origexception.h"
#include "notimplemented.h"
#include "property.h"
#include "valueiteratorimpl.h"
#include "hashiterator.h"
#include "invaliditerator.h"
#include "traverseiterator.h"
#include "iteratorimpl.h"
#include "classimpl.h"
#include "objectimpl.h"
#include "parametersimpl.h"
#include "extensionimpl.h"
#include "compileroptions.h"
#include "executestate.h"
#include "opcodes.h"
#include "functor.h"
#include "constantimpl.h"
#include "delayedfree.h"
#include "extensionpath.h"
#include "symbol.h"
#include "module.h"

#ifndef ZVAL_COPY_VALUE
#define ZVAL_COPY_VALUE(z, v)  \
  do {                         \
    (z)->value = (v)->value;   \
    Z_TYPE_P(z) = Z_TYPE_P(v); \
  } while (0)
#endif

#ifndef INIT_PZVAL_COPY
#define INIT_PZVAL_COPY(z, v)  \
  do {                         \
    ZVAL_COPY_VALUE(z, v);     \
    Z_SET_REFCOUNT_P(z, 1);    \
    Z_UNSET_ISREF_P(z);        \
  } while (0)
#endif
