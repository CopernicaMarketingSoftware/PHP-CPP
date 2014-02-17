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
#include <map>
#include <memory>
#include <set>
#include <exception>

/**
 *  Include all headers files that are related to this library
 */
#include <phpcpp/type.h>
#include <phpcpp/value.h>
#include <phpcpp/array.h>
#include <phpcpp/hiddenpointer.h>
#include <phpcpp/globals.h>
#include <phpcpp/argument.h>
#include <phpcpp/byval.h>
#include <phpcpp/byref.h>
#include <phpcpp/global.h>
#include <phpcpp/hashmember.h>
#include <phpcpp/parameters.h>
#include <phpcpp/membermodifier.h>
#include <phpcpp/function.h>
#include <phpcpp/properties.h>
#include <phpcpp/base.h>
#include <phpcpp/method.h>
#include <phpcpp/member.h>
#include <phpcpp/public.h>
#include <phpcpp/protected.h>
#include <phpcpp/private.h>
#include <phpcpp/const.h>
#include <phpcpp/members.h>
#include <phpcpp/classmodifier.h>
#include <phpcpp/class.h>
#include <phpcpp/classinfo.h>
#include <phpcpp/extension.h>
#include <phpcpp/exception.h>

/**
 *  Macro to export a function
 */
#if defined(__GNUC__) && __GNUC__ >= 4
#   define PHPCPP_EXPORT __attribute__ ((visibility("default")))
#else
#   define PHPCPP_EXPORT
#endif

#endif /* phpcpp.h */
