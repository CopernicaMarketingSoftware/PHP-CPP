/**
 *  callable.cpp
 *
 *  A wrapper to handle a callback coming from
 *  within PHP
 *
 *  @author Martijn Otto <martijn.otto@copernica.com>
 *  @copyright 2016 Copernica B.V.
 */
#include "includes.h"

/**
 *  Start namespace
 */
namespace Php {

/**
 *  Retrieve pointer to the object
 *
 *  @param  execute_data    The current execution scope
 *  @return Pointer (as void because this cannot be templated!)
 */
Base *ZendCallable::instance(struct _zend_execute_data *execute_data)
{
    // find the object implementation and retrieve the base object
    return ObjectImpl::find(getThis())->object();
}

/**
 *  Retrieve the input parameters for the function
 *
 *  @param  execute_data    The current execution scope
 *  @return The input parameters
 */
Parameters ZendCallable::parameters(struct _zend_execute_data *execute_data)
{
    // parse and return the parameters
    return ParametersImpl{ getThis(), ZEND_NUM_ARGS() TSRMLS_CC };
}

/**
 *  Handle exceptions
 *
 *  @param  exception   The exception to handle
 */
void ZendCallable::handle(Exception &exception)
{
    // pass it on to the exception handler
    process(exception);
}

/**
 *  Yield (return) the given value
 *
 *  @param  return_value    The return_value to set
 *  @param  value           The value to return to PHP
 */
void ZendCallable::yield(struct _zval_struct *return_value, std::nullptr_t value)
{
    // set the return value to null
    RETVAL_NULL();
}

/**
 *  Yield (return) the given value
 *
 *  @param  return_value    The return_value to set
 *  @param  value           The value to return to PHP
 */
void ZendCallable::yield(struct _zval_struct *return_value, const Php::Value &value)
{
    // copy the value over to the return value
    RETVAL_ZVAL(value._val, 1, 0);
}

/**
 *  End namespace
 */
}
