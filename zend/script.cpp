/**
 *  Script.cpp
 *
 *  Implementation file for the script class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
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
 *  Helper function to compile the source code
 *  @param  name        name of the script
 *  @param  script      actual PHP code
 *  @param  size        length of the string
 *  @return opcodes
 */
zend_op_array *Script::compile(const char *name, const char *phpcode, size_t size)
{
    // Sadly, there is not a simple Zend function to compile a string into opcodes,
    // so we basically copy the code that we found in zend_execute_API.c inside
    // the zend_eval_stringl() function into this file here. However, the code
    // found there is full of zval manipulation, for which we can use the much
    // simpler Php::Value object
    Php::Value source(phpcode, size);
    
    // remember the old compiler options, and set new compiler options
    CompilerOptions options(ZEND_COMPILE_DEFAULT_FOR_EVAL);
    
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // compile the string
    return zend_compile_string(source._val, (char *)name TSRMLS_CC);
}

/**
 *  Constructor
 *  @param  name        name of the script
 *  @param  script      actual PHP code
 *  @param  size        length of the string
 */
Script::Script(const char *name, const char *phpcode, size_t size) : _opcodes(compile(name, phpcode, size))
{
}
    
/**
 *  End of namespace
 */
}

