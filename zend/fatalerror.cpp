/**
 *  FatalError.cpp
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
 *  Report this error as a fatal error
 *  @return bool
 */
bool FatalError::report() const
{
    // report the error
    zend_error(E_ERROR, "%s", what());
    
    // return true: it was reported
    return true;
}
    
/**
 *  End of namespace
 */
}

