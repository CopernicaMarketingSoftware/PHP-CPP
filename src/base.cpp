/**
 *  Base.cpp
 *
 *  Implementation file for the base of all classes
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Store the object in the PHP object cache
 *  @param  entry               Class entry
 *  @param  tsrm_ls
 *  @return MixedObject
 */
//MixedObject *Base::store(zend_class_entry *entry TSRMLS_DC)
//{
//    // allocate memory for the object
//    MixedObject *result = (MixedObject *)emalloc(sizeof(MixedObject));
//    
//    // store the new c++ object
//    result->cpp = this;
//    
//    // store the class entry in the newly created object
//    result->php.ce = entry;
//    
//    // initialize the object
//    zend_object_std_init(&result->php, entry TSRMLS_CC);
//
//#if PHP_VERSION_ID < 50399
//
//    // tmp variable
//    zval *tmp;
//    
//    // initialize the properties, php 5.3 way
//    zend_hash_copy(result->php.properties, &entry->default_properties, (copy_ctor_func_t) zval_property_ctor, &tmp, sizeof(zval*));
//
//#else
//
//    // version higher than 5.3 have an easier way to initialize
//    object_properties_init(&result->php, entry);
//
//#endif    
//
//#ifdef ZTS
//
//    // when in thread safety mode, the destruct method and free method have
//    // an extra parameter holding thread information
//    using DestructType = void(zend_object*,unsigned int,void***);
//    using FreeType = void(zend_object*,void***);
//    
//#else
//
//    // not in thread mode: no special parameter for the tsrm_ls variable
//    using DestructType = void(zend_object*,unsigned int);
//    using FreeType = void(zend_object*);
//    
//#endif
//    
//    // store the two destruct methods in temporary vars
//    DestructType *destructMethod = &ClassImpl::destructObject;
//    FreeType *freeMethod = &ClassImpl::freeObject;
//
//    // the destructor and clone handlers are set to NULL. I dont know why, but they do not
//    // seem to be necessary...
//    _handle = zend_objects_store_put(result, (zend_objects_store_dtor_t)destructMethod, (zend_objects_free_object_storage_t)freeMethod, NULL TSRMLS_CC);
//    
//    // done
//    return result;
//}

/**
 *  Overridable method that is called right before an object is destructed
 */
void Base::__destruct() const
{
    // throw exception, so that the PHP-CPP library will check if the user
    // somehow registered an explicit __destruct method
    throw NotImplemented();
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
 *  End namespace
 */
}

