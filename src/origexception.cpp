/**
 *  Implementation of the exception that was originally thrown by PHP
 *  code or the zend engine, and that could or could not be picked
 *  up by C++ code
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Destructor
 */
OrigException::~OrigException() noexcept
{
    // skip if the exception was restored
    if (_restored) return;
    
    // clean up the exception, because it was handled in C++ code
    zend_clear_exception();
}

/**
 *  End of namespace
 */
}

