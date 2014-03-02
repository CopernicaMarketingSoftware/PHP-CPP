/**
 *  Countable.cpp
 *
 *  Implementation of the Countable interface
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

#include "spl/spl_iterators.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Implementation of the countable interface
 *  @return zend_class_entry*
 *  @internal
 */
struct _zend_class_entry *Countable::implementation()
{
    return spl_ce_Countable;
}

/**
 *  End namespace
 */
}

