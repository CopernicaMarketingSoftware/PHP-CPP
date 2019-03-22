/**
 *  Exception_handler.cpp
 *
 *  Set the exception handler
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 */

/**
 *  Dependencies
 */
#include "includes.h"
#include "string.h"

/**
 *  Open the PHP namespace
 */
namespace Php {

/**
 *  Set a std::function as a php exception handler
 */
Value set_exception_handler(const std::function<Value(Parameters &params)> &handler)
{
    // create a functor which wraps our callback
    Function functor(handler);

    // initialize our output value
    Value output;

    // turn our user_exception_handler into a Value so we can return the original one later on
    if (!Z_ISNULL(EG(user_exception_handler))) output = &EG(user_exception_handler);

    // detach so we have the zval
    auto value = functor.detach(true);

    // copy our zval into the user_exception_handler
    ZVAL_COPY(value, &EG(user_exception_handler));

    // return the original handler
    return output;
}

/**
 *  Set a std::function as a php error handler
 */
Value set_error_handler(const std::function<Value(Parameters &params)> &handler, Message message)
{
    // create the functor which wraps our callback
    Function functor(handler);

    // initialize our output value
    Value output;

    // turn our user_error_handler into a Value if we have one, just so we can return it later on
    if (!Z_ISNULL(EG(user_error_handler))) output = &EG(user_error_handler);

    // detach so we have the zval
    auto value = functor.detach(true);

    // copy our zval into the user_error_handler
    ZVAL_COPY_VALUE(&EG(user_error_handler), value);
    EG(user_error_handler_error_reporting) = (int) message;

    // return the original handler
    return output;
}

/**
 *  Modify the error reporting level, will return the old error reporting level.
 */
Value error_reporting(Message message)
{
    // store the old error reporting value
    Value output(EG(error_reporting));

    // create a small temporary buffer
    char str[21];

    // write the level into this buffer
    int size = sprintf(str, "%d", (int) message);

    // if we failed for some reason we bail out
    if (size < 0) return false;

    // the entry to change - static to avoid unnecessary extra allocations
    static String entry{ "error_reporting" };

    // alter the ini on the fly
    zend_alter_ini_entry(entry, String(str, size), ZEND_INI_USER, ZEND_INI_STAGE_RUNTIME);

    // return the output
    return output;
}

/**
 *  End of namespace
 */
}
