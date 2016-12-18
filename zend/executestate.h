/**
 *  ExecuteState.h
 *
 *  Class that keeps the current execution state (this is used when a different
 *  script or file is eval'ed to store the execution state so that we can switch
 *  back to the original state.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

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
     *  @var mixed
     */
    zend_op_array *_active_op_array;
    zval *_return_value;
    const zend_op *_opline;

    /**
     *  The new value for 'no-extensions'
     *  @var int
     */
    int _no_extensions;

public:
    /**
     *  No trivial constructor
     */
    ExecuteState() = delete;

    /**
     *  Constructor
     *  @param  no_extensions
     */
    ExecuteState(int no_extensions)
    {
        // store all the original stuff
        _active_op_array = CG(active_op_array);
        _return_value  = EG(current_execute_data)->return_value;
        _opline = EG(current_execute_data)->opline;
        _no_extensions = no_extensions;
    }

    /**
     *  Destructor
     */
    virtual ~ExecuteState()
    {
        // restore all settings
        EG(no_extensions) = _no_extensions;
        EG(current_execute_data)->opline = _opline;
        CG(active_op_array) = _active_op_array;
        EG(current_execute_data)->return_value = _return_value;
    }
};

/**
 *  End of namespace
 */
}

