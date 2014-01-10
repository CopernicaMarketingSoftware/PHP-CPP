/**
 *  Implementation of the exception that was originally thrown by PHP
 *  code or the zend engine, and that could or could not be picked
 *  up by C++ code
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"
#include <php.h>
#include "zend_exceptions.h"

void cpp_throw_exception_hook(struct _zval_struct *exception)
{
  throw Php::OrigException(exception);
}

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  zval
 */
OrigException::OrigException(struct _zval_struct *zval) : 
    Value(zval), 
    Exception("OrigException"), 
    _restored(false)
{
    std::cout << "save the exception" << std::endl;
    
    // save the exception
    zend_exception_save();
}

/**
 *  Copy constructor
 *  @param  exception
 */
OrigException::OrigException(const OrigException &exception) : 
    Value(exception), 
    Exception("OrigException"),
    _restored(exception._restored) {}

/**
 *  Move constructor
 *  @param  exception
 */
OrigException::OrigException(OrigException &&exception) : 
    Value(std::move(exception)), 
    Exception("OrigException"),
    _restored(exception._restored) {}

/**
 *  Destructor
 */
OrigException::~OrigException()
{
    // skip if the exception was restored
    if (_restored) return;
    zend_throw_exception_hook = NULL;
    // clean up the exception
    zend_clear_exception();
}
                                         

/**
 *  Restore the exception
 *  @internal
 */
void OrigException::restore()
{
    std::cout << "restore the exception" << std::endl;
    
    // restore the exception
    zend_exception_restore();
    
    // mark exception as restored
    _restored = true;
}

void OrigException::hook()
{
  zend_throw_exception_hook = cpp_throw_exception_hook;
}
                                            
                                            
/**
 *  End of namespace
 */
}

