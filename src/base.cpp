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
    // wrap the properties table, as a reference
    return Object(*_object->properties_table, true);
}

/**
 *  End of namespace
 */
}

