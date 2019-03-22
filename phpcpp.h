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
#include <phpcpp/visibility.h>
#include <phpcpp/deprecated.h>
#include <phpcpp/noexcept.h>
#include <phpcpp/thread_local.h>
#include <phpcpp/platform.h>
#include <phpcpp/version.h>
#include <phpcpp/inivalue.h>
#include <phpcpp/ini.h>
#include <phpcpp/throwable.h>
#include <phpcpp/exception.h>
#include <phpcpp/error.h>
#include <phpcpp/streams.h>
#include <phpcpp/message.h>
#include <phpcpp/type.h>
#include <phpcpp/hashparent.h>
#include <phpcpp/value.h>
#include <phpcpp/valueiterator.h>
#include <phpcpp/array.h>
#include <phpcpp/object.h>
#include <phpcpp/globals.h>
#include <phpcpp/argument.h>
#include <phpcpp/byval.h>
#include <phpcpp/byref.h>
#include <phpcpp/global.h>
#include <phpcpp/hashmember.h>
#include <phpcpp/super.h>
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
#include <phpcpp/constant.h>
#include <phpcpp/interface.h>
#include <phpcpp/zendcallable.h>
#include <phpcpp/class.h>
#include <phpcpp/namespace.h>
#include <phpcpp/extension.h>
#include <phpcpp/call.h>
#include <phpcpp/script.h>
#include <phpcpp/file.h>
#include <phpcpp/function.h>
#include <phpcpp/stream.h>

#endif /* phpcpp.h */
