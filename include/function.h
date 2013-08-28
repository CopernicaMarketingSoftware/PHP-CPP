/**
 *  Function.h
 *
 *  Object represents a callable function that is defined with the CPP API.
 *  After you've instantiated the extension, you can add function objects to
 *  it.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
 
/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Forward definitions
 */
class Callable;

/** 
 *  Class definition
 */
class Function
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the function
     *  @param  arguments   The arguments that can be passed to the function
     */
    Function(const std::string &name, const std::initializer_list<Argument> &arguments);

    /**
     *  Constructor
     *  @param  name        Name of the function
     */
    Function(const char *name) : Function(name, {}) {}
    
    /**
     *  Copy constructor
     *  @param  function    The other function
     */
    Function(const Function &function)
    {
        // copy other object
        _refcount = function._refcount;
        _callable = function._callable;
        
        // increate number of references
        (*_refcount)++;
    }
    
    /**
     *  Destructor
     */
    virtual ~Function()
    {
        // cleanup the object
        cleanup();
    }
    
    /**
     *  Assignment operator
     *  @param  function    The other function
     *  @return Function
     */
    Function &operator=(const Function &function)
    {
        // skip self assignment
        if (&function == this) return *this;
        
        // cleanup the object
        cleanup();
        
        // copy other object
        _refcount = function._refcount;
        _callable = function._callable;
        
        // increate number of references
        (*_refcount)++;
        
        // done
        return *this;
    }

    /**
     *  Method that gets called every time the function is executed
     *  @param  request     The request during which the call was made
     *  @param  arguments   The actual arguments that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(const Request *request, const std::initializer_list<Value> &arguments)
    {
    }
    
    /**
     *  Get access to the internal object
     *  @return Callable
     *  @internal
     */
    Callable *internal() const
    {
        return _callable;
    }

protected:
    /**
     *  Pointer to the callable object
     *  @var smart_ptr
     */
    Callable *_callable;
    
    /**
     *  Counter with the number of references
     *  @var integer
     */
    int *_refcount;


    /**
     *  Remove one reference
     */
    void cleanup();
};

/**
 *  End of namespace
 */
}

