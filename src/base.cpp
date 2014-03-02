/**
 *  Base.cpp
 *
 *  Implementation of the base class
 *
 *  @documentation private
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Convert the object to a Php::Value object (how it is used externally)
 *  @return Value
 */
Value Base::value() const
{
    // because the object is stored in a MixedObject, we know that the zend_object
    // structure is right in front of the this pointer
    zend_object *object = (zend_object *)this - sizeof(zend_object);
    
    // wrap the properties table, as a reference
    return Object(*object->properties_table, true);
}

/**
 *  End of namespace
 */
}

