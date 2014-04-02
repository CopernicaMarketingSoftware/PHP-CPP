/**
 *  phpcpp.h
 *
 *  Library to build PHP extensions with CPP
 * 
 *  @copyright 2013 CopernicA BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

#ifndef PHPCPP_H
#define PHPCPP_H

/**
 *  Other C and C++ libraries that PhpCpp depends on
 */
#include <string.h>
#include <string>
#include <initializer_list>
#include <vector>
#include <memory>
#include <list>
#include <exception>
#include <map>

/**
 *  Include PHP config
 */
#include <phpcpp/config.h>

/**
 *  Is ZTS enabled?
 */
#ifdef ZTS

    // enable TSRM
#   define TSRMLS_C     tsrm_ls
#   define TSRMLS_CC    ,tsrm_ls
#   define TSRMLS_D     void ***tsrm_ls
#   define TSRMLS_DC    ,void ***tsrm_ls

#else

    // disable TSRM
#   define TSRMLS_C    
#   define TSRMLS_CC    
#   define TSRMLS_D
#   define TSRMLS_DC    

#endif

/**
 *  Include all headers files that are related to this library
 */
#include <phpcpp/exception.h>
#include <phpcpp/streams.h>
#include <phpcpp/type.h>
#include <phpcpp/hashparent.h>
#include <phpcpp/value.h>
#include <phpcpp/valueiterator.h>
#include <phpcpp/array.h>
#include <phpcpp/object.h>
#include <phpcpp/hiddenpointer.h>
#include <phpcpp/globals.h>
#include <phpcpp/argument.h>
#include <phpcpp/byval.h>
#include <phpcpp/byref.h>
#include <phpcpp/global.h>
#include <phpcpp/super.h>
#include <phpcpp/hashmember.h>
#include <phpcpp/parameters.h>
#include <phpcpp/modifiers.h>
#include <phpcpp/base.h>
#include <phpcpp/countable.h>
#include <phpcpp/arrayaccess.h>
#include <phpcpp/iterator.h>
#include <phpcpp/traversable.h>
#include <phpcpp/serializable.h>
#include <phpcpp/classtype.h>
#include <phpcpp/classbase.h>
#include <phpcpp/interface.h>
#include <phpcpp/class.h>
#include <phpcpp/namespace.h>
#include <phpcpp/extension.h>
#include <phpcpp/call.h>

/**
 *  Macro to export a function
 */
#if defined(__GNUC__) && __GNUC__ >= 4
#   define PHPCPP_EXPORT __attribute__ ((visibility("default")))
#else
#   define PHPCPP_EXPORT
#endif

#endif /* phpcpp.h */
