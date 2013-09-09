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
 *  Forward definitions
 */
struct _zend_function_entry;
struct _zend_internal_function_info;
 
/**
 *  Set up namespace
 */
namespace Php {

/** 
 *  Class definition
 */
class Function
{
public:

//    Function(std::function<Value()> &function);
//    Function(std::function<Value(Value&)> &function);
//    Function(std::function<Value(Value&,Value&)> &function);
//    Function(std::function<Value(Value&,Value&,Value&)> &function);
//    Function(std::function<Value(Value&,Value&,Value&,Value&)> &function);
//    Function(std::function<void()> &function);
//    Function(std::function<void(Value&)> &function);
//    Function(std::function<void(Value&,Value&)> &function);
//    Function(std::function<void(Value&,Value&,Value&)> &function);
//    Function(std::function<void(Value&,Value&,Value&,Value&)> &function);

    /**
     *  Constructor
     *  @param  min     Min number of arguments
     *  @param  max     Max number of arguments
     */
    Function(int min = 0, int max = 0)
    {
        // construct the arguments
        _arguments = std::shared_ptr<Arguments>(new Arguments(min, max));
    }
    
    /**
     *  No copy constructor
     *  @param  function    The other function
     */
    Function(const Function &function)
    {
        // copy members
        _arguments = function._arguments;
        _type = function._type;
    }

    /**
     *  Move constructor
     *  @param  function    The other function
     */
    Function(Function &&function)
    {
        // copy arguments
        _arguments = function._arguments;
        _type = function._type;

        // no longer need the other arguments
        function._arguments.reset();
    }
    
    /**
     *  Destructor
     */
    virtual ~Function() {}
    
    /**
     *  Assignment operator
     *  @param  function    The other function
     *  @return Function
     */
    Function &operator=(const Function &function)
    {
        // skip self reference
        if (this == &function) return *this;
        
        // copy members
        _arguments = function._arguments;
        _type = function._type;
        
        // done
        return *this;
    }

    /**
     *  Method that gets called every time the function is executed
     *  @param  request     Request object
     *  @param  params      The parameters that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(Request &request, Parameters &params)
    {
        return 0;
    }

protected:
    /**
     *  Suggestion for the return type
     *  @var    Type
     */
    Type _type = nullType;

    /**
     *  Pointer to the arguments
     *  @var    shared_ptr
     */
    std::shared_ptr<Arguments> _arguments;

private:
    /**
     *  Fill a function entry
     *  @param  name        Name of the function
     *  @param  entry       Entry to be filled
     */
    void fill(const char *name, struct _zend_function_entry *entry);

    /**
     *  Fill function info
     *  @param  name        Name of the function
     *  @param  info        Info object to be filled
     */
    void fill(const char *name, struct _zend_internal_function_info *info);
    
    /**
     *  Extension has access to the private members
     */
    friend class Extension;

};

/**
 *  End of namespace
 */
}

