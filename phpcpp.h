/**
 *  phpcpp.h
 *
 *  Library to build PHP extensions with CPP
 *
 *  @copyright 2013 - 2019 Copernica BV
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
#include <set>
#include <functional>

/**
 *  Include all headers files that are related to this library
 */
#include <include/visibility.h>
#include <include/deprecated.h>
#include <include/noexcept.h>
#include <include/thread_local.h>
#include <include/platform.h>
#include <include/version.h>
#include <include/inivalue.h>
#include <include/ini.h>
#include <include/throwable.h>
#include <include/exception.h>
#include <include/error.h>
#include <include/streams.h>
#include <include/message.h>
#include <include/type.h>
#include <include/hashparent.h>
#include <include/value.h>
#include <include/valueiterator.h>
#include <include/array.h>
#include <include/object.h>
#include <include/globals.h>
#include <include/argument.h>
#include <include/byval.h>
#include <include/byref.h>
#include <include/global.h>
#include <include/hashmember.h>
#include <include/super.h>
#include <include/parameters.h>
#include <include/modifiers.h>
#include <include/base.h>
#include <include/countable.h>
#include <include/arrayaccess.h>
#include <include/iterator.h>
#include <include/traversable.h>
#include <include/serializable.h>
#include <include/classtype.h>
#include <include/classbase.h>
#include <include/constant.h>
#include <include/interface.h>
#include <include/zendcallable.h>
#include <include/class.h>
#include <include/namespace.h>
#include <include/extension.h>
#include <include/call.h>
#include <include/script.h>
#include <include/file.h>
#include <include/function.h>
#include <include/stream.h>

#endif /* phpcpp.h */
