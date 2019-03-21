/**
 *  Opcodes.h
 *
 *  Class represents a set of opcodes of a PHP script that can be executed. This
 *  is an internal file that you normally do not have to instantiate yourself.
 *  Better use the Php::Script of Php::File classes.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 - 2019 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_op_array;

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Opcodes
{
public:
    /**
     *  Constructor
     *  @param  opcodes
     */
    Opcodes(struct _zend_op_array *opcodes) : _opcodes(opcodes)
    {
    }

    /**
     *  Destructor
     */
    virtual ~Opcodes()
    {
        // leap out if opcodes were not valid
        if (!_opcodes) return;

        // clean up opcodes
        destroy_op_array(_opcodes);
        efree(_opcodes);
    }

    /**
     *  Are the opcodes valid?
     *  @return bool
     */
    bool valid() const
    {
        return _opcodes != nullptr;
    }

    /**
     *  Execute the opcodes
     *  @return Value
     */
    Value execute() const
    {
        // if the script could not be compiled, we return null
        if (!_opcodes) return nullptr;

        // pointer that is going to hold the return value of the script
        zval retval;

        // initialize to null
        ZVAL_NULL(&retval);

        // the zend engine is probably already busy processing opcodes, so we store
        // the current execute state before we're going to switch the runtime to
        // our own set of opcodes
        ExecuteState execState(0);

        // old execute state has been saved (and will automatically be restored when
        // the oldstate is destructed), so we can now safely overwrite all the settings
        CG(active_op_array) = _opcodes;
        EG(no_extensions) = 1;
        if (!EG(current_execute_data)->symbol_table) zend_rebuild_symbol_table();

        // the current exception state
        State state;

        // execute the code
        zend_execute(_opcodes, &retval);

        // was an exception thrown inside the eval()'ed code? In that case we
        // throw a C++ new exception to give the C++ code the chance to catch it
        state.rethrow();

        // we're ready if there is no return value
        if (ZVAL_IS_NULL(&retval)) return nullptr;

        // wrap the return value
        Value result(&retval);

        // copy the pointer into a value object, and return that
        return result;
    }

private:
    /**
     *  The opcodes
     *  @var zend_op_array
     */
    struct _zend_op_array *_opcodes;
};

/**
 *  End of namespace
 */
}

