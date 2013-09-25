/**
 *  InternalFunction.h
 *
 *  Helper union to create an internal function
 *
 *  @documentation private
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  An internal function
 */
class InternalFunction
{
public:
    /**
     *  Constructor
     *  @param  handler
     *  @param  flags
     */
    InternalFunction(void (*handler)(INTERNAL_FUNCTION_PARAMETERS), int flags = 0)
    {
        // set everything to zero
        memset(&_func, 0, sizeof(zend_internal_function));
        
        // set the appropriate properties
        _func.type = ZEND_INTERNAL_FUNCTION;
        _func.handler = handler;
        _func.fn_flags = flags;

//        _func.function_name = NULL;
//        _func.scope = NULL;
//        _func.prototype = NULL;
//        _func.num_args = 0;
//        _func.required_num_args = 0;
//        _func.arg_info = NULL;
//        _func.module = NULL;
    }
    
    /**
     *  Destructor
     */
    virtual ~InternalFunction() {}
    
    /**
     *  Cast to zend_internal_function pointer
     *  @return zend_internal_function
     */
    operator zend_internal_function *()
    {
        return &_func;
    }
    
    /**
     *  Cast to zend_function pointer
     *  @return zend_function
     */
    operator zend_function *()
    {
        return (zend_function *)&_func;
    }

private:
    /**
     *  The internal function object
     *  @var    zend_internal_function
     */
    zend_internal_function _func;
};


/**
 *  End of namespace
 */
}

