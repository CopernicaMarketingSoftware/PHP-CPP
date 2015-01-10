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
#include <mutex>

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Helper class to temporarily set compiler options
 * 
 *  When an object is destructed, it automatically restored the previous compiler settings
 */
class CompilerOptions
{
private:
    /**
     *  The original compiler options
     *  @var int
     */
    zend_uint _original;

public:
    /**
     *  Constructor
     *  @param  options
     */
    CompilerOptions(zend_uint options)
    {
        // remember the old compiler options before we set temporary compile options
        _original = CG(compiler_options);
        
        // we're going to evaluate only once
        CG(compiler_options) = options;
    }
    
    /**
     *  Destructor
     */
    virtual ~CompilerOptions()
    {
        // restore original options
        CG(compiler_options) = _original;
    }
};

/**
 *  Helper class to store and restore the current opcode state
 * 
 *  When we're going to execute a set of instructions, we need to store the
 *  current state of the Zend engine. After the instructions have been processed,
 *  we can switch back to the original instructions
 */
class ExecuteState
{
private:
    /**
     *  All the original settings
     */
    zend_op_array *_active_op_array;
    zval **_return_value_ptr_ptr;
    zend_op **_opline_ptr;
    int _interactive;
    
public:
    /**
     *  Constructor
     */
    ExecuteState()
    {
        // store all the original stuff
        _active_op_array = EG(active_op_array);
        _return_value_ptr_ptr = EG(return_value_ptr_ptr);
        _opline_ptr = EG(opline_ptr);
        _interactive = CG(interactive);
    }

    /**
     *  Destructor
     */
    virtual ~ExecuteState()
    {
        // restore all settings
        CG(interactive) = _interactive;
        EG(no_extensions) = 0;
        EG(opline_ptr) = _opline_ptr;
        EG(active_op_array) = _active_op_array;
        EG(return_value_ptr_ptr) = _return_value_ptr_ptr;
    }
};
    
/**
 *  Constructor
 *  @param  name        name of the script
 *  @param  script      actual PHP code
 *  @param  size        length of the string
 */
Script::Script(const char *name, const char *phpcode, size_t size)
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
    _opcodes = zend_compile_string(source._val, (char *)name TSRMLS_CC);
}
    
/**
 *  Destructor
 */
Script::~Script()
{
    // do nothing if there are no opcodes
    if (!_opcodes) return;
    
    // clean up opcodes
    destroy_op_array(_opcodes TSRMLS_CC);
    efree(_opcodes);
}

/**
 *  Execute a script
 *  @return Value
 */
Value Script::execute() const
{
    // if the script could not be compiled, we return null
    if (!_opcodes) return nullptr;

    // pointer that is going to hold the return value of the script
    zval *retval_ptr = nullptr;

    // the zend engine is probably already busy processing opcodes, so we store
    // the current execute state before we're going to switch the runtime to
    // our own set of opcodes
    ExecuteState oldstate;
    
    // old execute state has been saved (and will automatically be restured when
    // the oldstate is destructed), so we can now safely overwrite all the settings
    EG(return_value_ptr_ptr) = &retval_ptr;
    EG(active_op_array) = _opcodes;
    EG(no_extensions) = 1;
    if (!EG(active_symbol_table)) zend_rebuild_symbol_table(TSRMLS_C);
    CG(interactive) = 0;
    
    // this code was copied from zend_execute_API.c. this is what I think it does:
    // the zend_execute() call could result in all sort of disastrous things, one
    // of them is a full crash of the executing script. if that happens, the zend
    // engine does a long jump right up to some other point in the code. the
    // 'zend_try' and 'zend_catch' macros prevent this: they install a new 
    // destination for the long jump, so that we can catch a failure
    zend_try 
    {
        // execute the code
        zend_execute(_opcodes TSRMLS_CC);
    } 
    zend_catch
    {
        // the code could not be executed, and the zend engine is in big
        // trouble now, in the original code the _opcodes are efree'd, but here
        // we can just as well continue with the real bailout (the zend_try/
        // zend_catch pair was maybe not even necessary???)
        zend_bailout();
    }
    zend_end_try();

    // we're ready if there is no return value
    if (!retval_ptr) return nullptr;
    
    // copy the pointer into a value object, and return that
    return retval_ptr;
}
    
/**
 *  End of namespace
 */
}

