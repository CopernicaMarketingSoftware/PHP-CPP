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

    // we need the tsrm_ls variable
    // @todo it would be better if this was passed as param to the compile() method
    TSRMLS_FETCH();
    
    // remember the old compiler options, and set new compiler options
    CompilerOptions options(ZEND_COMPILE_DEFAULT_FOR_EVAL TSRMLS_CC);
    
    // compile the string
    return zend_compile_string(source._val, (char *)name TSRMLS_CC);
}

/**
 *  Constructor
 *  @param  name        name of the script
 *  @param  script      actual PHP code
 *  @param  size        length of the string
 */
Script::Script(const char *name, const char *phpcode, size_t size) _NOEXCEPT
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // construct opcodes
    _opcodes = new Opcodes(compile(name, phpcode, size) TSRMLS_CC);
}

/**
 *  Destructor
 */
Script::~Script()
{
    // remove opcodes
    delete _opcodes;
}

/**
 *  Is the script a valid PHP script without syntax errors?
 *  @return bool
 */
bool Script::valid() const
{
    // check opcodes
    return _opcodes && _opcodes->valid();
}

/**
 *  Execute the script
 *  The return value of the script is returned
 *  @return Value
 */
Value Script::execute() const
{
    // pass on to opcodes
    if (!_opcodes) return nullptr;
    
    // execute opcodes
    return _opcodes->execute();
}

/**
 *  End of namespace
 */
}

