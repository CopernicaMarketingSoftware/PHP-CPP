/**
 *  Zval.cpp
 *
 *  Implementation for the Zval class
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  In case this is a reference, dereference it into a zval.. In case it isn't
 *  the actual value is returned.
 *  @return struct _zval_struct*
 */
struct _zval_struct *Zval::dereference() const
{
    // first cast ourselves to a zval struct
    struct _zval_struct *self = const_cast<struct _zval_struct*>(reinterpret_cast<const struct _zval_struct*>(&_buffer));

    // if we're not a reference we just return ourselves as is
    if (((Type) Z_TYPE_P(self)) != Type::Reference) return self;

    // otherwise we simply retrieve the value of the reference
    return Z_REFVAL_P(self);
}

/**
 *  End namespace
 */
}