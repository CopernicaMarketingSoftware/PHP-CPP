/**
 *  NativeFunction.h
 *
 *  The NativeFunction class is an extension of the Function class that
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
class NativeFunction : public Function
{
public:
    /**
     *  Constructor
     *  @param  name            Function name
     *  @param  function        The native C function
     */
    NativeFunction(const char *name, native_callback_0 function, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(0) { _function.f0 = function; }
    NativeFunction(const char *name, native_callback_1 function, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(1) { _function.f1 = function; }
    NativeFunction(const char *name, native_callback_2 function, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(2) { _function.f2 = function; }
    NativeFunction(const char *name, native_callback_3 function, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(3) { _function.f3 = function; }

    /**
     *  Destructor
     */
    virtual ~NativeFunction() {}

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

