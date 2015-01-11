/**
 *  Opcodes.cpp
 *
 *  Implementation file for the opcodes class
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
 *  Destructor
 */
Opcodes::~Opcodes()
{
    // leap out if opcodes were not valid
    if (!_opcodes) return;
    
    // clean up opcodes
    destroy_op_array(_opcodes TSRMLS_CC);
    efree(_opcodes);
}

/**
 *  Execute the opcodes
 *  @return Value
 */
Value Opcodes::execute() const
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
    
    // the current exception
    zval* oldException = EG(exception);

    // execute the code
    zend_execute(_opcodes TSRMLS_CC);

    // was an exception thrown inside the eval()'ed code? In that case we 
    // throw a C++ new exception to give the C++ code the chance to catch it
    if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

    // we're ready if there is no return value
    if (!retval_ptr) return nullptr;
    
    // wrap the return value
    Value result(retval_ptr);
    
    // destruct the zval (this function will decrement the reference counter,
    // and only destruct if there are no other references left)
    zval_ptr_dtor(&retval_ptr);
    
    // copy the pointer into a value object, and return that
    return result;
}

/**
 *  End of namespace
 */
}

