/**
 *  Opcodes.h
 *
 *  Class represents a set of opcodes of a PHP script that can be executed. This
 *  is an internal file that you normally do not have to instantiate yourself.
 *  Better use the Php::Script of Php::File classes.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
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
    Opcodes(struct _zend_op_array *opcodes TSRMLS_DC) : _opcodes(opcodes) 
    {
#ifdef ZTS
        // copy tsrm_ls param
        this->tsrm_ls = tsrm_ls;
#endif
    }
    
    /**
     *  Destructor
     */
    virtual ~Opcodes()
    {
        // leap out if opcodes were not valid
        if (!_opcodes) return;
        
        // clean up opcodes
        destroy_op_array(_opcodes TSRMLS_CC);
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
        zval *retval_ptr = nullptr;
        
        // the zend engine is probably already busy processing opcodes, so we store
        // the current execute state before we're going to switch the runtime to
        // our own set of opcodes
        ExecuteState oldstate(TSRMLS_C);
        
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

private:
    /**
     *  The opcodes
     *  @var zend_op_array
     */
    struct _zend_op_array *_opcodes;

#ifdef ZTS
    /**
     *  When in thread safety mode, we also keep track of the TSRM_LS var
     *  @var void***
     */
    void ***tsrm_ls;
#endif

};
    
/**
 *  End of namespace
 */
}

