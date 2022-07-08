/**
 *  Base.cpp
 *
 *  Implementation file for the base of all classes
 *
 *  @copyright 2014 - 2022 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Overridable method that is called right before an object is destructed
 */
void Base::__destruct() const
{
    // destroy the object by default
    zend_objects_destroy_object(_impl->php());
}

/**
 *  Overridable method that is called to check if a property is set
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @return bool
 */
bool Base::__isset(const Php::Value &key) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the unset function can be called
    throw NotImplemented();
}

/**
 *  Overridable method that is called to set a new property
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @param  value
 */
void Base::__set(const Php::Value &key, const Php::Value &value) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the unset function can be called
    throw NotImplemented();
}

/**
 *  Retrieve a property
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @return value
 */
Php::Value Base::__get(const Php::Value &key) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}

/**
 *  Remove a member
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param key
 */
void Base::__unset(const Php::Value &key) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
}

/**
 *  Call a method
 * 
 *  This method is called when a method is called from the PHP script that
 *  was not explicitly defined. You can use this to catch variable method
 *  names, or to support all thinkable method names.
 * 
 *  @param  method      Name of the method that was called
 *  @param  params      The parameters that were passed to the function
 *  @return Value       The return value
 */
Value Base::__call(const char *method, Parameters &params) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}

/**
 *  Call the class as if it was a function
 * 
 *  This method is called when a an object is used with () operators:
 *  $object(). You can override this method to make objects callable.
 * 
 *  @param  params      The parameters that were passed to the function
 *  @return Value       The return value
 */
Value Base::__invoke(Parameters &params) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}

/**
 *  Cast the object to a string
 * 
 *  This method is called when an object is casted to a string, or when
 *  it is used in a string context
 * 
 *  @return Value       The object as a string
 */
Value Base::__toString() const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}    

/**
 *  Cast the object to an integer
 * 
 *  This method is called when an object is casted to an integer, or when
 *  it is used in an integer context
 * 
 *  @return int         Integer value
 */
Value Base::__toInteger() const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return 0;
}

/**
 *  Cast the object to a float
 * 
 *  This method is called when an object is casted to a float, or when it
 *  is used in a float context
 * 
 *  @return double      Floating point value
 */
Value Base::__toFloat() const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return 0.0;
}

/**
 *  Cast the object to a boolean
 * 
 *  This method is called when an object is casted to a bool, or when it
 *  is used in a boolean context
 * 
 *  @return bool
 */
Value Base::__toBool() const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return false;
}

/**
 *  Compare the object with a different object
 *  
 *  Check how a different object compares to this object
 * 
 *  @param  that        Object to compare with
 *  @return int
 */
int Base::__compare(const Base &that) const
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return 1;
}

/**
 *  Method that is called when an explicit call to $object->serialize() is made
 *  Note that a call to serialize($object) does not end up in this function, but
 *  is handled by the user-space implementation of Serializable::serialize()).
 *  @return Php::Value
 */
Php::Value Base::__serialize()
{
    // 'this' refers to a Php::Base class, but we expect that is also implements the Serializable
    // interface (otherwise we would never have registered the __serialize function as a callback)
    auto *serializable = dynamic_cast<Serializable*>(this);

    // this one should not fail
    if (serializable == nullptr) return "";

    // pass the call to the interface
    return serializable->serialize();
}

/**
 *  Method that is called when an explicit call to $object->unserialize() is made
 *  Note that a call to unserialize($string) does not end up in this function, but
 *  is handled by the user-space implementation of Serializable::unserialize()).
 *  @param params       The passed parameters
 */
void Base::__unserialize(Php::Parameters &params)
{
    // 'this' refers to a Php::Base class, but we expect that is also implements the Serializable
    // interface (otherwise we would never have registered the __serialize function as a callback)
    auto *serializable = dynamic_cast<Serializable*>(this);

    // this one should not fail
    if (serializable == nullptr) return;

    // the passed in parameter
    Php::Value param = params[0];

    // make sure the parameter is indeed a string
    param.setType(Type::String);

    // pass the call to the interface
    serializable->unserialize(param.rawValue(), param.size());
}

/**
 *  Method that is called when an explicit call to $object->count() is made
 *  Note that a call to unserialize($string) does not end up in this function, but
 *  is handled by the user-space implementation of Serializable::count()).
 *  @param params       The passed parameters
 */
Php::Value Base::__count(Php::Parameters &params)
{
    // 'this' refers to a Php::Base class, but we expect that is also implements the Countable
    // interface (otherwise we would never have registered the __count function as a callback)
    auto *countable = dynamic_cast<Countable*>(this);

    // this one should not fail
    if (countable == nullptr) return 0;

    // pass the call to the interface
    return countable->count();
}

/**
 *  End namespace
 */
}

