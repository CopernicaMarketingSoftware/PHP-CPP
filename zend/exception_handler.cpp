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
    if (EG(user_exception_handler)) output = EG(user_exception_handler);

    // detach so we have the zval
    auto value = functor.detach(true);

    // allocate the user_exception_handler
    ALLOC_ZVAL(EG(user_exception_handler));

    // copy our zval into the user_exception_handler
    MAKE_COPY_ZVAL(&value, EG(user_exception_handler));

    // return the original handler
    return output;
}

Value set_error_handler(const std::function<Value(Parameters &params)> &handler)
{
    // create the functor which wraps our callback
    Function functor(handler);

    // initialize our output value
    Value output;

    // turn our user_error_handler into a Value if we have one, just so we can return it later on
    if (EG(user_error_handler)) output = EG(user_error_handler);

    // detach so we have the zval
    auto value = functor.detach(true);

    // alocate the user_error_handler
    ALLOC_ZVAL(EG(user_error_handler));

    // copy our zval into the user_error_handler
    MAKE_COPY_ZVAL(&value, EG(user_error_handler));
    EG(user_error_handler_error_reporting) = (int) (E_ALL | E_STRICT);

    // return the original handler
    return output;
}

/**
 *  End of namespace
 */
}
