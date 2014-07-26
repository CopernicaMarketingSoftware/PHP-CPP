/**
 *  Eval.cpp
 *
 *  This file holds the implementation for the Php::eval() function
 * 
 *  @author andot <https://github.com/andot>
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Evaluate a PHP string
 *  @param  phpCode     The PHP code to evaluate
 *  @return Value       The result of the evaluation
 */
Value eval(const std::string &phpCode) 
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the current exception
    zval* oldException = EG(exception);

    // the return zval
    zval* retval = nullptr;
    if (zend_eval_stringl_ex((char *)phpCode.c_str(), (int32_t)phpCode.length(), retval, (char *)"", 1 TSRMLS_CC) != SUCCESS)
    {
        // throw an exception, php couldn't evaluate code
        throw Exception("PHP eval error");

        // unreachable, but let's return at least something to prevent compiler warnings
        return nullptr;
    }
    else
    {
        // was an exception thrown inside the function? In that case we throw a C++ new exception
        // to give the C++ code the chance to catch it
        if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

        // no (additional) exception was thrown
        return retval ? Value(retval) : nullptr;
    }
}

/**
 *  End of namespace
 */
}
