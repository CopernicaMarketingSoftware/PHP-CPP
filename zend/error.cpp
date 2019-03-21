/**
 *  Error.cpp
 * 
 *  Implementation file for the Error class
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Begin of namespace
 */
namespace Php {
    
/**
 *  Rethrow the exception / make sure that it ends up in PHP space
 */
void Error::rethrow()
{
    // add the exception to userspace
    zend_throw_exception(zend_ce_error, what(), _code);
}

/**
 *  End of namespace
 */
}

