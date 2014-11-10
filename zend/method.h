/**
 *  Method.h
 *
 *  Internal class that represents a native class method, that can be called
 *  from PHP scripts.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Method : public Callable
{
public:
    /**
     *  Constructor 
     * 
     *  @param  name            Method name
     *  @param  callback        Native callback
     *  @param  flags           Access flags
     *  @param  args            Argument description
     *  @param  return_ref      Return reference or not
     */
    Method(const char *name, const method_callback_0 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(0),    _flags(flags) { _callback.m0  = callback; }
    Method(const char *name, const method_callback_1 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(1),    _flags(flags) { _callback.m1  = callback; }
    Method(const char *name, const method_callback_2 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(2),    _flags(flags) { _callback.m2  = callback; }
    Method(const char *name, const method_callback_3 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(3),    _flags(flags) { _callback.m3  = callback; }
    Method(const char *name, const method_callback_4 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(4),    _flags(flags) { _callback.m4  = callback; }
    Method(const char *name, const method_callback_5 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(5),    _flags(flags) { _callback.m5  = callback; }
    Method(const char *name, const method_callback_6 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(6),    _flags(flags) { _callback.m6  = callback; }
    Method(const char *name, const method_callback_7 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(7),    _flags(flags) { _callback.m7  = callback; }
    Method(const char *name, const native_callback_0 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(8),    _flags(flags) { _callback.m8  = callback; }
    Method(const char *name, const native_callback_1 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(9),    _flags(flags) { _callback.m9  = callback; }
    Method(const char *name, const native_callback_2 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(10),   _flags(flags) { _callback.m10 = callback; }
    Method(const char *name, const native_callback_3 &callback, int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(11),   _flags(flags) { _callback.m11 = callback; }
    Method(const char *name,                                    int flags, const Arguments &args, bool return_ref = false) : Callable(name, args, return_ref), _type(9999), _flags(flags) { _callback.m0 = nullptr;  }

    /**
     *  Copy and move constructors
     *  @param  that
     */
    Method(const Method &that) : Callable(that), _type(that._type), _flags(that._flags), _callback(that._callback) {}
    Method(Method &&that) : Callable(std::move(that)), _type(that._type), _flags(that._flags), _callback(that._callback) {}

    /**
     *  Destructor
     *  @param  type
     *  @param  callback
     */
    virtual ~Method() {}

    /**
     *  Internal method to fill a function entry
     *  @param  zend_function_entry
     *  @param  classname
     */
    void initialize(struct _zend_function_entry *entry, const std::string &classname)
    {
        // fix the flags, if neither public, private and protected is set, we use public,
        // (this solves php warnings if only "final" or only "abstract" is set
        if ((_flags & (Public|Private|Protected)) == 0) _flags |= Public;
        
        // call base
        Callable::initialize(entry, classname.c_str(), _flags);
    }

    /**
     *  Invoke the method
     *  @param  parameters
     *  @return Value
     */
    virtual Value invoke(Parameters &parameters) override
    {
        // the object to call a method on
        Base *base = parameters.object();
        
        // find out which method to call, and call it
        switch (_type) {
        case 0:     (base->*_callback.m0)(); return Value();
        case 1:     (base->*_callback.m1)(parameters); return Value();
        case 2:     return (base->*_callback.m2)();
        case 3:     return (base->*_callback.m3)(parameters);
        case 4:     (base->*_callback.m4)(); return Value();
        case 5:     (base->*_callback.m5)(parameters); return Value();
        case 6:     return (base->*_callback.m6)();
        case 7:     return (base->*_callback.m7)(parameters);
        case 8:     _callback.m8(); return Value();
        case 9:     _callback.m9(parameters); return Value();
        case 10:    return _callback.m10();
        case 11:    return _callback.m11(parameters);
        default:    return Value();
        }
    }


private:
    /**
     *  Callback type
     *  @var int
     */
    int _type;

    /**
     *  Access flags (protected, public, abstract, final, private, etc)
     *  @var int
     */
    int _flags;
    
    /**
     *  The actual callback
     *  @var void*
     */
    union {
         method_callback_0  m0;
         method_callback_1  m1;
         method_callback_2  m2;
         method_callback_3  m3;
         method_callback_4  m4;
         method_callback_5  m5;
         method_callback_6  m6;
         method_callback_7  m7;
         native_callback_0  m8;
         native_callback_1  m9;
         native_callback_2  m10;
         native_callback_3  m11;
    } _callback;
};

/**
 *  End of namespace
 */
}

