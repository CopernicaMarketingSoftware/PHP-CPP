/**
 *  eval.php
 *
 *  Implementation for eval
 *
 *  @copyright 2014 Copernica BV
 */

#include "includes.h"

namespace Php {

    Value eval(const std::string &phpCode) {
        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        // the current exception
        zval* oldException = EG(exception);

        // the return zval
        zval* retval = nullptr;
        if (zend_eval_stringl_ex((char *)phpCode.c_str(), phpCode.length(), retval, (char *)"", 1 TSRMLS_CC) != SUCCESS)
        {
            // throw an exception, php couldn't evaluate code
            throw Exception("PHP couldn't evaluate: " + phpCode);

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
}