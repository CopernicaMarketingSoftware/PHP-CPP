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
 *  Check whether we have received valid parameters
 *
 *  If this function returns false a warning will have been
 *  generated and the return value has been set to NULL.
 *
 *  @param  execute_data    The current execution scope
 *  @param  return_value    The return value to set on failure
 */
bool ZendCallable::valid(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
{
    // how many parameters are required and how many to we have?
    auto required = execute_data->func->common.required_num_args;
    auto provided = ZEND_NUM_ARGS();

    // if we have the required minimum number of arguments there is no problem
    if (provided >= required) return true;

    // retrieve the function name to display the error
    auto *name = get_active_function_name();

    // we do not have enough input parameters, show a warning about this
    Php::warning << name << "() expects at least " << required << " parameter(s), " << provided << " given" << std::flush;

    // set the return value to NULL
    RETVAL_NULL();

    // we are not in a valid state
    return false;
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
    return ParametersImpl{ getThis(), ZEND_NUM_ARGS() };
}

/**
 *  Handle throwables
 *  @param  throwable       The object to handle
 */
void ZendCallable::handle(Throwable &throwable)
{
    // pass to user space
    throwable.rethrow();
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
