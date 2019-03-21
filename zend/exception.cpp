/**
 *  Exception.cpp
 * 
 *  Implementation file for the Exception class
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
void Exception::rethrow()
{
    // add the exception to userspace
    zend_throw_exception(zend_ce_exception, what(), _code);
}

/**
 *  End of namespace
 */
}

