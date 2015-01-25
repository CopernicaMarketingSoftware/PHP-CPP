/**
 *  ConstantFuncs.cpp
 *
 *  C++ implementation of PHP functions to retrieve and set constants.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Retrieve the value of a constant by its name
 *  @param  name            Name of the constant
 *  @return Value           Actual constant value
 */
Value constant(const char *name)
{
    // pass on to other implementation
    return constant(name, ::strlen(name));
}

/**
 *  Retrieve a constant by its name, and the size of the name
 *  @param  constant        Name of the constant
 *  @param  size            Size of the name
 *  @return Value
 */
Value constant(const char *constant, size_t size)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the value that holds the result
    Value result;

    // retrieve the constant
    if (!zend_get_constant(constant, size, result._val TSRMLS_CC)) return nullptr;
    
    // zval was correctly retrieved, wrap in value
    return result;
}

/**
 *  Retrieve the value of a constant by its name
 *  @param  name            Name of the constant
 *  @return Value           Actual constant value
 */
Value constant(const std::string &name)
{
    // pass on to other implementation
    return constant(name.c_str(), name.size());
}

/**
 *  Check whether a constant exists
 *  @param  name
 *  @param  size
 *  @return bool
 */
bool defined(const char *name, size_t size) 
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // result variable
    zval c;

    // retrieve the constant
    if (!zend_get_constant_ex(name, size, &c, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) return false;

    // constant exists, but the returned zval should first be destructed
    zval_dtor(&c);
    
    // done
    return true;
}

/**
 *  Check whether a constant exists
 *  @param  name
 *  @return bool
 */
bool defined(const char *name)
{
    // pass on
    return defined(name, ::strlen(name));
}

/**
 *  Check whether a constant exists
 *  @param  name
 *  @return bool
 */
bool defined(const std::string &name)
{
    // pass on
    return defined(name.c_str(), name.size());
}

/**
 *  End namespace
 */
}

