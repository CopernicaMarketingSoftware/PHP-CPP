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
    zval **_return_value_ptr_ptr;
    zend_op **_opline_ptr;
    int _interactive;
    
    /**
     *  The new value for 'no-extensions'
     *  @var int
     */
    int _no_extensions;

#ifdef ZTS
    /**
     *  When in thread safety mode, we also keep track of the TSRM_LS var
     *  @var void***
     */
    void ***tsrm_ls;
#endif
    
public:
    /**
     *  No trivial constructor
     */
    ExecuteState() = delete;

    /**
     *  Constructor
     *  @param  no_extensions
     */
    ExecuteState(int no_extensions TSRMLS_DC)
    {
        // store all the original stuff
        _active_op_array = EG(active_op_array);
        _return_value_ptr_ptr = EG(return_value_ptr_ptr);
        _opline_ptr = EG(opline_ptr);
        _interactive = CG(interactive);
        _no_extensions = no_extensions;

#ifdef ZTS
        // copy tsrm_ls param
        this->tsrm_ls = tsrm_ls;
#endif
    }

    /**
     *  Destructor
     */
    virtual ~ExecuteState()
    {
        // restore all settings
        CG(interactive) = _interactive;
        EG(no_extensions) = _no_extensions;
        EG(opline_ptr) = _opline_ptr;
        EG(active_op_array) = _active_op_array;
        EG(return_value_ptr_ptr) = _return_value_ptr_ptr;
    }
};

/**
 *  End of namespace
 */
}

