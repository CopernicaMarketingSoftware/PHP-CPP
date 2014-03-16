/**
 *  Includes.h
 *
 *  Startup include file to compile the phpcpp library
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Include standard C and C++ libraries
 */
#include <stdlib.h>
#include <string>
#include <initializer_list>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <exception>
#include <type_traits>

 // for debug
#include <iostream>

/**
 *  @todo: if ZTS defined many errors appear. need debug.
 */
//#define ZTS 1

/**
 *  PHP includes
 */
#pragma GCC system_header
#include <php.h>
#include "zend_exceptions.h"
#include "zend_interfaces.h"

/**
 *  Macro to convert results to success status
 */
#define BOOL2SUCCESS(b) ((b) ? SUCCESS : FAILURE)

/**
 *  Include other files from this library
 */
#include "../include/exception.h"
#include "../include/type.h"
#include "../include/value.h"
#include "../include/valueiterator.h"
#include "../include/array.h"
#include "../include/object.h"
#include "../include/hiddenpointer.h"
#include "../include/globals.h"
#include "../include/argument.h"
#include "../include/byval.h"
#include "../include/byref.h"
#include "../include/global.h"
#include "../include/super.h"
#include "../include/hashmember.h"
#include "../include/parameters.h"
#include "../include/modifiers.h"
#include "../include/base.h"
#include "../include/countable.h"
#include "../include/arrayaccess.h"
#include "../include/serializable.h"
#include "../include/iterator.h"
#include "../include/traversable.h"
#include "../include/classtype.h"
#include "../include/classbase.h"
#include "../include/class.h"
#include "../include/interface.h"
#include "../include/namespace.h"
#include "../include/extension.h"
#include "../include/call.h"
#include "../include/init.h"

/**
 *  Interface files for internal use only
 */
#include "mixedobject.h"
#include "callable.h"
#include "function.h"
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
