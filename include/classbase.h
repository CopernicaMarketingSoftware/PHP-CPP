/**
 *  ClassBase.h
 *
 *  This is the base class of the "Class" class. This is an internal class that
 *  is used by the PHP-CPP library. Because the constructor is protected,
 *  you can not create any instances if this class yourself (and you are not
 *  supposed to do that either).
 *
 *  Further more, because this base class is a 'private' base of Class, all 
 *  features of it are normally also inaccessible.
 *
 *  In other words: it is not meant to be directly used by extension writers.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  A couple of predefined native callback functions that can be registered.
 *  These are functions that optional accept a Request and/or Parameters object,
 *  and that either return void or a Value object. 
 */
typedef void    (*native_callback_0)();
typedef void    (*native_callback_1)(Parameters &);
typedef Value   (*native_callback_2)();
typedef Value   (*native_callback_3)(Parameters &);

/**
 *  Method signatures
 */
typedef void    (Base::*method_callback_0)();
typedef void    (Base::*method_callback_1)(Parameters &);
typedef Value   (Base::*method_callback_2)();
typedef Value   (Base::*method_callback_3)(Parameters &);
typedef void    (Base::*method_callback_4)() const;
typedef void    (Base::*method_callback_5)(Parameters &) const;
typedef Value   (Base::*method_callback_6)() const;
typedef Value   (Base::*method_callback_7)(Parameters &) const;

/**
 *  Signatures for getters and setters
 */
typedef Value   (Base::*getter_callback_0)();
typedef Value   (Base::*getter_callback_1)() const;
typedef void    (Base::*setter_callback_0)(const Php::Value &value);
typedef void    (Base::*setter_callback_1)(const Php::Value &value) const;

/**
 *  Forward declarations
 */
class ClassImpl;

/**
 *  Class definition
 */
class ClassBase
{
protected:
    /**
     *  Protected constructor
     *  @param  classname   Class name
     *  @param  flags       Class flags
     */
    ClassBase(const char *classname, int flags);
        
    /**
     *  Protected constructor
     *  @param  classname   Class name
     *  @param  type        Class type
     */
    ClassBase(const char *classname, ClassType type);
    
public:
    /**
     *  Copy constructor
     *  @param  that
     */
    ClassBase(const ClassBase &that) : _impl(that._impl) {}

    /**
     *  Move constructor
     *  @param  that
     */
    ClassBase(ClassBase &&that) : _impl(std::move(that._impl)) {}

    /**
     *  Destructor
     */
    virtual ~ClassBase() {}

    /**
     *  Construct a new instance of the object, or to clone the object
     *  @return Base
     */
    virtual Base* construct()       const { return nullptr; }
    virtual Base *clone(Base *orig) const { return nullptr; }

    /**
     *  Methods to check if a certain interface is overridden, or a copy
     *  constructor is available
     *  @return bool
     */
    virtual bool traversable()  const { return false; }
    virtual bool serializable() const { return false; }
    virtual bool clonable()     const { return false; }

    /**
     *  Compare two objects
     *  @param  object1
     *  @param  object2
     *  @return int
     */
    virtual int callCompare(Base *object1, Base *object2) const { return 1; }
    
    /**
     *  Call the __clone and __destruct magic methods
     *  @param  base
     */
    virtual void callClone(Base *base) const {}
    virtual void callDestruct(Base *base) const {}
    
    /**
     *  Call the __call(), __invoke() or __callStatic() method
     *  @param  base        Object to call on
     *  @param  name        Name of the method
     *  @param  params      Parameters to pass to the method
     *  @return Value
     */
    virtual Value callCall(Base *base, const char *name, Parameters &params) const { return nullptr; }
    virtual Value callInvoke(Base *base, Parameters &params) const { return nullptr; }
    virtual Value callCallStatic(const char *name, Parameters &params) const { return nullptr; }
    
    /**
     *  Casting functions
     *  @param  base
     *  @return Value
     */
    virtual Value callToString(Base *base) const { return Value(Type::String); }
    virtual Value callToInteger(Base *base) const { return Value(Type::Numeric); }
    virtual Value callToFloat(Base *base) const { return Value(Type::Float); }
    virtual Value callToBool(Base *base) const { return Value(Type::Bool); }
    
    /**
     *  Function to get and set properties
     *  @param  base
     *  @param  name
     *  @param  value
     *  @return Value
     */
    virtual Value callGet(Base *base, const Value &name) const { return nullptr; }
    virtual void  callSet(Base *base, const Value &name, const Value &value) const {}
    virtual void  callUnset(Base *base, const Value &name) const {}
    virtual bool  callIsset(Base *base, const Value &name) const { return false; }
    
    /**
     *  Get access to the implementation object
     *  @return std::shared_ptr
     */
    const std::shared_ptr<ClassImpl> &implementation() const { return _impl; }

protected:
    /**
     *  Function that can be called by a derived method when a certain function
     *  is not implemented
     */
    static void notImplemented();

    /**
     *  Add a method to the class
     *  
     *  The method will be accessible as one of the class methods in your PHP
     *  code. When the method is called, it will automatically be forwarded
     *  to the C++ implementation. The flags can be Php::Public, Php::Protected
     *  or Php::Private (using private methods can be useful if you for example
     *  want to make the __construct() function private). The access-modified
     *  flag can be bitwise combined with the flag Php::Final or Php::Abstract).
     * 
     *  @param  name        Name of the method
     *  @param  method      The actual method
     *  @param  flags       Optional flags
     *  @param  args        Description of the supported arguments
     *  @param  return_ref  Return reference or not
     */
    void method(const char *name, const method_callback_0 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_1 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_2 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_3 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_4 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_5 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_6 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const method_callback_7 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);

    /**
     *  Add a static method to the class
     * 
     *  Because a C++ static method is just a regular function, that happens to
     *  have access to the private variables of the class at compile time, you
     *  can register any function that matches one of the function signatures
     *  
     *  @param  name        Name of the method
     *  @param  method      The actual method
     *  @param  flags       Optional flags
     *  @param  args        Description of the supported arguments
     *  @param  return_ref  Return reference or not
     */
    void method(const char *name, const native_callback_0 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const native_callback_1 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const native_callback_2 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);
    void method(const char *name, const native_callback_3 &method, int flags=0, const Arguments &args = {}, bool return_ref = false);

    /**
     *  Add an abstract method to the class
     * 
     *  @param  name        Name of the method
     *  @param  flags       Optional flags (like public or protected)
     *  @param  args        Description of the supported arguments
     *  @param  return_ref  Return reference or not
     */
    void method(const char *name, int flags=0, const Arguments &args = {}, bool return_ref = false);

    /**
     *  Add a property to the class
     * 
     *  Every instance of this class will have this property. The property
     *  can be Php::Public, Php::Protected or Php::Private (altough setting
     *  private properties is odd as the implementation of the class is in CPP,
     *  so why use private properties while the whole implementation is already
     *  hidden)
     * 
     *  @param  name        Name of the property
     *  @param  value       Actual property value
     *  @param  flags       Optional flags
     */
    void property(const char *name, std::nullptr_t value, int flags = Php::Public);
    void property(const char *name, int16_t value, int flags = Php::Public);
    void property(const char *name, int32_t value, int flags = Php::Public);
    void property(const char *name, int64_t value, int flags = Php::Public);
    void property(const char *name, bool value, int flags = Php::Public);
    void property(const char *name, char value, int flags = Php::Public);
    void property(const char *name, const std::string &value, int flags = Php::Public);
    void property(const char *name, const char *value, int flags = Php::Public);
    void property(const char *name, double value, int flags = Php::Public);

    /**
     *  Set property with callbacks
     *  @param  name        Name of the property
     *  @param  getter      Getter methode
     *  @param  setter      Setter method
     */
    void property(const char *name, const getter_callback_0 &getter);
    void property(const char *name, const getter_callback_1 &getter);
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_0 &setter);
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_0 &setter);
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_1 &setter);
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_1 &setter);

    /**
     *  Add an interface
     *  @param  interface       Interface object
     */
    void implements(const ClassBase &interface);

    /**
     *  Set the base class
     *  @param  base            Php::Class object that is the base
     */
    void extends(const ClassBase &base);

private:
    /**
     *  Pointer to the actual implementation
     *  @var    std::shared_ptr<ClassImpl>
     */
    std::shared_ptr<ClassImpl> _impl;
};
    
/**
 *  End namespace
 */
}

