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

    // the return va
    zval *retval = nullptr;
    MAKE_STD_ZVAL(retval);
    
    // evaluate the string
    if (zend_eval_stringl_ex((char *)phpCode.c_str(), (int32_t)phpCode.length(), retval, (char *)"", 1 TSRMLS_CC) != SUCCESS)
    {
        // Do we want to throw an exception here? The original author of this code
        // did, but there are some reasons not to:
        //
        //  1. the PHP eval() function also does not throw exceptions.
        //
        //  2. the zend_eval_string() function already triggers a 
        //     'PHP parse error' when an error occurs, which also has
        //     to be handled. If we also throw an exception here, the
        //     user will have to write two error checks: for the error
        //     and the exception.
        //
        // if we _do_ want to throw an exception, we will first have to
        // prevent the original zend_error to occur, and then turn it
        // into an exception. An exception would be nicer from a C++
        // point of view, but because of the extra complexity, we do not
        // this for now.
        return nullptr;
    }
    else
    {
        // was an exception thrown inside the eval()'ed code? In that case we 
        // throw a C++ new exception to give the C++ code the chance to catch it
        if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

        // wrap the return value in a value object
        Value result(retval);

        // the retval should now have two references: the value object and the
        // retval itselves, so we can remove one of it (the zval_ptr_dtor only
        // decrements refcount and won't destruct anything because there still 
        // is one reference left inside the Value object)
        zval_ptr_dtor(&retval);
        
        // done
        return result;
    }
}

/**
 *  Include a file
 *  @param  filename
 *  @return Value
 */
Value include(const std::string &filename)
{
    // we can simply execute a file
    return File(filename).execute();
}

/**
 *  Include a file only once
 *  @param  filename
 *  @return Value
 */
Value include_once(const std::string &filename)
{
    // we can simply execute a file
    return File(filename).execute();
}

/**
 *  Require a file
 *  This causes a fatal error if the file does not exist
 *  @param  filename
 *  @return Value
 */
Value require(const std::string &filename)
{
    // create the file
    File file(filename);
    
    // execute if it exists
    if (file.exists()) return file.execute();
    
    // trigger fatal error
    error << filename << " does not exist" << std::flush;
    
    // unreachable
    return nullptr;
}

/**
 *  Require a file only once
 *  This causes a fatal error if the file does not exist
 *  @param  filename
 *  @return Value
 */
Value require_once(const std::string &filename)
{
    // create the file
    File file(filename);
    
    // execute if it exists
    if (file.exists()) return file.once();
    
    // trigger fatal error
    error << filename << " does not exist" << std::flush;
    
    // unreachable
    return nullptr;
}


/**
 *  End of namespace
 */
}
