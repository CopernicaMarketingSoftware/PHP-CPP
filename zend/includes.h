/**
 *  Includes.h
 *
 *  Startup include file to compile the phpcpp library
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 - 2019 Copernica BV
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
#include "../include/visibility.h"
#include "../include/deprecated.h"
#include "../include/noexcept.h"
#include "../include/thread_local.h"
#include "../include/platform.h"
#include "../include/version.h"
#include "../include/inivalue.h"
#include "../include/ini.h"
#include "../include/throwable.h"
#include "../include/exception.h"
#include "../include/error.h"
#include "../include/streams.h"
#include "../include/type.h"
#include "../include/message.h"
#include "../include/hashparent.h"
#include "../include/value.h"
#include "../include/valueiterator.h"
#include "../include/array.h"
#include "../include/object.h"
#include "../include/globals.h"
#include "../include/argument.h"
#include "../include/byval.h"
#include "../include/byref.h"
#include "../include/global.h"
#include "../include/hashmember.h"
#include "../include/super.h"
#include "../include/parameters.h"
#include "../include/modifiers.h"
#include "../include/base.h"
#include "../include/countable.h"
#include "../include/arrayaccess.h"
#include "../include/propertyptrptr.h"
#include "../include/serializable.h"
#include "../include/iterator.h"
#include "../include/traversable.h"
#include "../include/classtype.h"
#include "../include/classbase.h"
#include "../include/interface.h"
#include "../include/constant.h"
#include "../include/zendcallable.h"
#include "../include/class.h"
#include "../include/namespace.h"
#include "../include/extension.h"
#include "../include/call.h"
#include "../include/script.h"
#include "../include/file.h"
#include "../include/function.h"
#include "../include/stream.h"

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
#include "rethrowable.h"
#include "state.h"
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
