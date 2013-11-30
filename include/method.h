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
     *  @param  environment
     *  @param  parameters
     *  @return Value
     */
    Value invoke(Environment &environment, Parameters &parameters);

protected:
    /**
     *  Protected constructor to prevent that anyone instantiates this object
     */
    _Method(method_callback_0 callback) : _type(0) { _callback.m0 = callback; }
    _Method(method_callback_1 callback) : _type(1) { _callback.m1 = callback; }
    _Method(method_callback_2 callback) : _type(2) { _callback.m2 = callback; }
    _Method(method_callback_3 callback) : _type(3) { _callback.m3 = callback; }
    _Method(method_callback_4 callback) : _type(4) { _callback.m4 = callback; }
    _Method(method_callback_5 callback) : _type(5) { _callback.m5 = callback; }
    _Method(method_callback_6 callback) : _type(6) { _callback.m6 = callback; }
    _Method(method_callback_7 callback) : _type(7) { _callback.m7 = callback; }
    
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
         method_callback_4 m4;
         method_callback_5 m5;
         method_callback_6 m6;
         method_callback_7 m7;
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
    Method(void(T::*callback)(Environment&))                : _Method(static_cast<method_callback_2>(callback)) {}
    Method(void(T::*callback)(Environment&,Parameters&))    : _Method(static_cast<method_callback_3>(callback)) {}
    Method(Value(T::*callback)())                           : _Method(static_cast<method_callback_4>(callback)) {}
    Method(Value(T::*callback)(Parameters&))                : _Method(static_cast<method_callback_5>(callback)) {}
    Method(Value(T::*callback)(Environment&))               : _Method(static_cast<method_callback_6>(callback)) {}
    Method(Value(T::*callback)(Environment&,Parameters&))   : _Method(static_cast<method_callback_7>(callback)) {}
    
    /**
     *  Destructor
     */
    virtual ~Method() {}

};
    
/**
 *  End of namespace
 */
}


