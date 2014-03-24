/**
 *  Exception.cpp
 *
 *  Implementation for the exception class
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
 *  Process the exception
 * 
 *  This method is called only from within the PHP-CPP library,
 *  and will turn the exception into a PHP exception
 * 
 *  @param  tsrm_ls
 */
void Exception::process(TSRMLS_D)
{
    // an exception originally thrown by C++ should be passed on to PHP
    zend_throw_exception(zend_exception_get_default(TSRMLS_C), (char*)message().c_str(), 0 TSRMLS_CC);
}

/**
 *  End namespace
 */
}

