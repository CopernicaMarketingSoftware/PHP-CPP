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

/**
 *  End of namespace
 */
}
