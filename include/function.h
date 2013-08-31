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
     *  No copy constructor
     *  @param  function    The other function
     */
    Function(const Function &function) = delete;

    /**
     *  Move constructor
     *  @param  function    The other function
     */
    Function(Function &&function)
    {
        _callable = function._callable;
        function._callable = nullptr;
    }
    
    /**
     *  Destructor
     */
    virtual ~Function();
    
    /**
     *  No assignment operator
     *  @param  function    The other function
     *  @return Function
     */
    Function &operator=(const Function &function) {}

    /**
     *  Method that gets called every time the function is executed
     *  @param  request     The request during which the call was made
     *  @param  arguments   The actual arguments that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(const Request *request, const std::initializer_list<Value> &arguments);
    
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
};

/**
 *  End of namespace
 */
}

