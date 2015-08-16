/**
 *  @file phpcpp.h
 *
 *  This file provides the list of materials for building the final product
 *  which is the bridge between PHP and C++ (PHP-CPP)
 *
 *  @copyright 2013 CopernicA BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

#ifndef PHPCPP_H
#define PHPCPP_H

/**
 *  Platform dependencies
 */
#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>

/**
 *  Local library dependencies
 */
#include <phpcpp/visibility.h>
#include <phpcpp/noexcept.h>
#include <phpcpp/platform.h>
#include <phpcpp/version.h>
#include <phpcpp/inivalue.h>
#include <phpcpp/ini.h>
#include <phpcpp/exception.h>
#include <phpcpp/fatalerror.h>
#include <phpcpp/streams.h>
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
#include <phpcpp/class.h>
#include <phpcpp/namespace.h>
#include <phpcpp/extension.h>
#include <phpcpp/call.h>
#include <phpcpp/script.h>
#include <phpcpp/file.h>
#include <phpcpp/function.h>

#endif /* PHPCPP_H */
