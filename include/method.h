/**
 *  Method.h
 */
 
/**
 *  Namespace
 */
namespace Php {

/**
 *  A very generic function pointer
 */
typedef void (*function_ptr)();

/**
 *  Base class of the method
 */
class _Method
{
public:
    /**
     *  Copy constructor
     *  @param  method
     */
    _Method(const _Method &method) : _type(method._type), _callback(method._callback) {}

    /**
     *  Destructor
     *  @param  type
     *  @param  callback
     */
    virtual ~_Method() {}

    /**
     *  Invoke the method
     *  @param  parameters
     *  @return Value
     */
    Value invoke(Parameters &parameters)
    {
        // the object to call a method on
        Base *base = parameters.object();
        
        // find out which method to call, and call it
        switch (_type) {
        case 0:     (base->*_callback.m0)(); return Value();
        case 1:     (base->*_callback.m1)(parameters); return Value();
        case 2:     return (base->*_callback.m2)();
        case 3:     return (base->*_callback.m3)(parameters);
        default:    return Value();
        }
    }

protected:
    /**
     *  Protected constructor to prevent that anyone instantiates this object
     */
    _Method(method_callback_0 callback) : _type(0) { _callback.m0 = callback; }
    _Method(method_callback_1 callback) : _type(1) { _callback.m1 = callback; }
    _Method(method_callback_2 callback) : _type(2) { _callback.m2 = callback; }
    _Method(method_callback_3 callback) : _type(3) { _callback.m3 = callback; }
    
private:
    /**
     *  Callback type
     *  @var int
     */
    int _type;
    
    /**
     *  The actual callback
     *  @var void*
     */
    union {
         method_callback_0 m0;
         method_callback_1 m1;
         method_callback_2 m2;
         method_callback_3 m3;
    } _callback;
};

/**
 *  Actual template class of the method
 */
template <typename T>
class Method : public _Method
{
public:
    /**
     *  Constructor
     *  @param  callback
     */
    Method(void(T::*callback)())                            : _Method(static_cast<method_callback_0>(callback)) {}
    Method(void(T::*callback)(Parameters&))                 : _Method(static_cast<method_callback_1>(callback)) {}
    Method(Value(T::*callback)())                           : _Method(static_cast<method_callback_2>(callback)) {}
    Method(Value(T::*callback)(Parameters&))                : _Method(static_cast<method_callback_3>(callback)) {}
    
    /**
     *  Destructor
     */
    virtual ~Method() {}

};
    
/**
 *  End of namespace
 */
}


