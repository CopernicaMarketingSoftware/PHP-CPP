/**
 *  ClassBase.cpp
 *
 *  Implementation of the ClassBase class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Protected constructor
 *  @param  classname   Class name
 *  @param  flags       Class flags
 */
ClassBase::ClassBase(const char *classname, int flags)
{
    // the flags hold a method-flag-value, this should be converted into a class-type
    if (flags & Abstract) _impl = std::make_shared<ClassImpl>(classname, ClassType::Abstract);
    else if (flags & Final) _impl = std::make_shared<ClassImpl>(classname, ClassType::Final);
    else _impl = std::make_shared<ClassImpl>(classname, ClassType::Regular);
}

/**
 *  Protected constructor
 *  @param  classname   Class name
 *  @param  type        Class type
 */
ClassBase::ClassBase(const char *classname, ClassType type)
{
    // construct implementation
    _impl = std::make_shared<ClassImpl>(classname, type);
}

/**
 *  Function that can be called by a derived method when a certain function
 *  is not implemented
 */
void ClassBase::notImplemented()
{
    // throw an exception
    throw NotImplemented();
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_0 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_1 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_2 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_3 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_4 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_5 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_6 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }
void ClassBase::method(const char *name, const method_callback_7 &callback, int flags, const Arguments &args) { _impl->method(name, callback, flags, args); }

/**
 *  Add a static method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const native_callback_0 &method, int flags, const Arguments &args) { _impl->method(name, method, flags, args); }
void ClassBase::method(const char *name, const native_callback_1 &method, int flags, const Arguments &args) { _impl->method(name, method, flags, args); }
void ClassBase::method(const char *name, const native_callback_2 &method, int flags, const Arguments &args) { _impl->method(name, method, flags, args); }
void ClassBase::method(const char *name, const native_callback_3 &method, int flags, const Arguments &args) { _impl->method(name, method, flags, args); }

/**
 *  Add an abstract method to the class
 *  @param  name        Name of the method
 *  @param  flags       Optional flags (like public or protected)
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, int flags, const Arguments &args) { _impl->method(name, flags, args); }

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, std::nullptr_t value, int flags)     { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, int16_t value, int flags)            { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, int32_t value, int flags)            { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, int64_t value, int flags)            { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, bool value, int flags)               { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, char value, int flags)               { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, const std::string &value, int flags) { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, const char *value, int flags)        { _impl->property(name, value, flags); }
void ClassBase::property(const char *name, double value, int flags)             { _impl->property(name, value, flags); }

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 */
void ClassBase::property(const char *name, const getter_callback_0 &getter) { _impl->property(name, getter); }
void ClassBase::property(const char *name, const getter_callback_1 &getter) { _impl->property(name, getter); }
void ClassBase::property(const char *name, const getter_callback_0 &getter, const setter_callback_0 &setter) { _impl->property(name, getter, setter); }
void ClassBase::property(const char *name, const getter_callback_1 &getter, const setter_callback_0 &setter) { _impl->property(name, getter, setter); }
void ClassBase::property(const char *name, const getter_callback_0 &getter, const setter_callback_1 &setter) { _impl->property(name, getter, setter); }
void ClassBase::property(const char *name, const getter_callback_1 &getter, const setter_callback_1 &setter) { _impl->property(name, getter, setter); }

/**
 *  End namespace
 */
}

