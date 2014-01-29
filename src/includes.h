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
#include <set>
#include <exception>

 // for debug
#include <iostream>

/**
 *  PHP includes
 */
#include <php.h>
#include "zend_exceptions.h"
/**
 *  Macro to convert results to success status
 */
#define BOOL2SUCCESS(b) ((b) ? SUCCESS : FAILURE)

/**
 *  Include other files from this library
 */
#include "../include/type.h"
#include "../include/value.h"
#include "../include/array.h"
#include "../include/hiddenpointer.h"
#include "../include/globals.h"
#include "../include/argument.h"
#include "../include/byval.h"
#include "../include/byref.h"
#include "../include/global.h"
#include "../include/hashmember.h"
#include "../include/parameters.h"
#include "../include/function.h"
#include "../include/properties.h"
#include "../include/zend.h"
#include "../include/flag.h"
#include "../include/base.h"
#include "../include/method.h"
#include "../include/member.h"
#include "../include/public.h"
#include "../include/protected.h"
#include "../include/classconst.h"
#include "../include/members.h"
#include "../include/class.h"
#include "../include/classinfo.h"
#include "../include/extension.h"
#include "../include/exception.h"
#include "../include/init.h"

/**
 *  Interface files for internal use only
 */
#include "mixedobject.h"
#include "nativefunction.h"
#include "internalfunction.h"
#include "memberinfo.h"
#include "nullmember.h"
#include "longmember.h"
#include "boolmember.h"
#include "stringmember.h"
#include "doublemember.h"
#include "methodmember.h"
#include "arithmetic.h"
#include "origexception.h"

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
