/**
 *  Function.h
 *
 *  The Function class is an extension of the Callable class that
 *  forwards the function call directly to a native function in C/C++
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Function : public Callable
{
public:
    /**
     *  Constructor
     *  @param  name            Function name
     *  @param  function        The native C function
     *  @param  arguments       Information about the arguments
     *  @param  return_ref      Return reference or not
     */
    Function(const char *name, const native_callback_0 &function, const Arguments &arguments = {}, bool return_ref = false) : Callable(name, arguments, return_ref), _type(0) { _function.f0 = function; }
    Function(const char *name, const native_callback_1 &function, const Arguments &arguments = {}, bool return_ref = false) : Callable(name, arguments, return_ref), _type(1) { _function.f1 = function; }
    Function(const char *name, const native_callback_2 &function, const Arguments &arguments = {}, bool return_ref = false) : Callable(name, arguments, return_ref), _type(2) { _function.f2 = function; }
    Function(const char *name, const native_callback_3 &function, const Arguments &arguments = {}, bool return_ref = false) : Callable(name, arguments, return_ref), _type(3) { _function.f3 = function; }

    /**
     *  Copy constructor
     *  @param  that
     */
    Function(const Function &that) : Callable(that), _function(that._function), _type(that._type) {}

    /**
     *  Move constructor
     *  @param  that
     */
    Function(Function &&that) : Callable(std::move(that)), _function(that._function), _type(that._type) {}

    /**
     *  Destructor
     */
    virtual ~Function() {}

    /**
     *  Method that gets called every time the function is executed
     *  @param  params      The parameters that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(Parameters &params) override
    {
        switch (_type) {
        case 0:     _function.f0(); return Value();
        case 1:     _function.f1(params); return Value();
        case 2:     return _function.f2();
        case 3:     return _function.f3(params);
        default:    return Value();
        }
    }

    /**
     *  Fill a function entry
     *  @param  prefix      Active namespace prefix
     *  @param  entry       Entry to be filled
     */
    void initialize(const std::string &prefix, zend_function_entry *entry)
    {
        // if there is a namespace prefix, we should adjust the name
        if (prefix.size()) _ptr = HiddenPointer<Callable>(this, prefix+"\\"+(const char *)_ptr);
        
        // call base initialize
        Callable::initialize(entry);
    }

private:
    /**
     *  Union of supported callbacks
     *  One of the callbacks will be set 
     */
    union {
        native_callback_0 f0;
        native_callback_1 f1;
        native_callback_2 f2;
        native_callback_3 f3;
    } _function;
    
    /**
     *  The callback that is set
     *  @var integer
     */
    int _type;
};

/**
 *  End of namespace
 */
}

