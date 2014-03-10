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
 *  This method is called only from withing the PHP-CPP library,
 *  and will turn the exception into a PHP exception
 */
void Exception::process()
{
    // an exception originally thrown by C++ should be passed on to PHP
    zend_throw_exception(zend_exception_get_default(), (char*)message().c_str(), 0 TSRMLS_CC);
}

/**
 *  End namespace
 */
}

