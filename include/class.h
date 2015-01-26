/**
 *  Class.h
 *
 *  When a class is registered in the extension, you need this helper class
 *  for that.
 *
 *  The use of it is simple:
 *
 *  Extension::add(Class<YourClass>);
 *
 *  Note that YourClass must extend from Php::Object
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  changed by Valeriy Dmitriev <ufabiz@gmail.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition of the class
 */
template <typename T>
class Class : private ClassBase
{
public:
    /**
     *  Constructor
     * 
     *  The flags can be a combination of Php::Final and Php::Abstract.
     *  If no flags are set, a regular public class will be formed.
     * 
     *  @param  name        Name of the class
     *  @param  flags       Accessibility flags
     */
    Class(const char *name, int flags = 0) : ClassBase(name, flags) {}
    
    /**
     *  Copy constructor
     *  @param  that
     */
    Class(const Class<T> &that) : ClassBase(that) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Class(Class<T> &&that) _NOEXCEPT : ClassBase(std::move(that)) {}

    /**
     *  Destructor
     */
    virtual ~Class() {}
    
    /**
     *  Add a regular method to the class
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
     *  @param  args        Argument descriptions
     *  @return Class       Same object to allow chaining
     */
    Class<T> &method(const char *name, void  (T::*method)(),                         int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_0>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)(Parameters &params),       int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_1>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(),                         int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_2>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(Parameters &params),       int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_3>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)(),                                    const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_0>(method), Public, args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)(Parameters &params),                  const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_1>(method), Public, args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(),                                    const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_2>(method), Public, args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(Parameters &params),                  const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_3>(method), Public, args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)()                   const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_4>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)(Parameters &params) const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_5>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)()                   const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_6>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(Parameters &params) const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_7>(method), flags,  args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)()                   const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_4>(method), Public, args); return *this; }
    Class<T> &method(const char *name, void  (T::*method)(Parameters &params) const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_5>(method), Public, args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)()                   const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_6>(method), Public, args); return *this; }
    Class<T> &method(const char *name, Value (T::*method)(Parameters &params) const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_7>(method), Public, args); return *this; }

    /**
     *  Add a static method to a class
     * 
     *  In C++ a static method is in reality just a plain function, that at 
     *  compile time has access to private properties of the class that it is a 
     *  static member of. 
     * 
     *  Because a C++ static method is not a real method with a 'this' pointer, 
     *  it has the same signature as a normal C++ (non-method) function. Therefore,
     *  you can register real static member functions (&MyClass::myMethod) as well
     *  as normal functions (myFunction) as class methods.
     * 
     *  In PHP scripts, such functions will be callable as static class methods
     * 
     *  @param  name        Name of the method
     *  @param  method      The actual method
     *  @param  flags       Optional flags
     *  @param  args        Argument descriptions
     *  @return Class       Same object to allow chaining
     */
    Class<T> &method(const char *name, const native_callback_0 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); return *this; }
    Class<T> &method(const char *name, const native_callback_1 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); return *this; }
    Class<T> &method(const char *name, const native_callback_2 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); return *this; }
    Class<T> &method(const char *name, const native_callback_3 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); return *this; }
    Class<T> &method(const char *name, const native_callback_0 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); return *this; }
    Class<T> &method(const char *name, const native_callback_1 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); return *this; }
    Class<T> &method(const char *name, const native_callback_2 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); return *this; }
    Class<T> &method(const char *name, const native_callback_3 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); return *this; }

    /**
     *  Add an abstract method to the class
     * 
     *  This is only meaningful for classes that can be extended. Because the 
     *  method is abstract, you will not have to pass an implementation. You
     *  can pass in flags to mark the method as protected
     * 
     *  @param  name        Name of the method
     *  @param  flags       Optional flags
     *  @param  args        Argument descriptions
     *  @return Class       Same object to allow chaining
     */
    Class<T> &method(const char *name, int flags, const Arguments &args = {}) { ClassBase::method(name, flags  | Abstract, args); return *this; }
    Class<T> &method(const char *name,            const Arguments &args = {}) { ClassBase::method(name, Public | Abstract, args); return *this; }

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
     *  @param  value       Actual default property value
     *  @param  flags       Optional flags
     *  @return Class       Same object to allow chaining
     */
    Class<T> &property(const char *name, std::nullptr_t value,     int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, int64_t value,            int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, int32_t value,            int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, int16_t value,            int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, char value,               int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, const char *value,        int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, const std::string &value, int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, bool value,               int flags = Public) { ClassBase::property(name, value, flags); return *this; }
    Class<T> &property(const char *name, double value,             int flags = Public) { ClassBase::property(name, value, flags); return *this; }

    /**
     *  Create a class constant
     * 
     *  The class constant can be used in a php script as "ClassName::CONSTANT_NAME".
     *  It is a good programming practive to only use uppercase characters for
     *  constants.
     * 
     *  This is an alias for adding a class property with the "Php::Const" flag.
     * 
     *  @param  name        Name of the constant
     *  @param  value       Constant value
     *  @return Class       Same object to allow chaining
     */
    template <typename V>
    Class<T> &constant(const char *name, V value) { return property(name, value, Const); }

    /**
     *  Add a Php::Constant to a class to use it as a class constant
     *  
     *  @param  constant    The constant to add
     *  @return Class       Same object to allow chaining
     */
    Class<T> &constant(const Constant &constant) { constant.addTo(*this); return *this; }
    Class<T> &add(const Constant &constant) { constant.addTo(*this); return *this; }

    /**
     *  Properties as methods
     * 
     *  This is a smarter way for adding properties to a class. You can define
     *  a property and a method that gets called every time the property is
     *  set or unset.
     * 
     *  If you do not set a setter method, your property will be read-only.
     * 
     *  @param  name        Name of the property
     *  @param  getter      The getter method
     *  @param  setter      The setter method
     */
    Class<T> &property(const char *name, Value (T::*getter)()                                                   ) { ClassBase::property(name, static_cast<getter_callback_0>(getter)); return *this; }
    Class<T> &property(const char *name, Value (T::*getter)() const                                             ) { ClassBase::property(name, static_cast<getter_callback_1>(getter)); return *this; }
    Class<T> &property(const char *name, Value (T::*getter)()      , void (T::*setter)(const Value &value)      ) { ClassBase::property(name, static_cast<getter_callback_0>(getter), static_cast<setter_callback_0>(setter)); return *this; }
    Class<T> &property(const char *name, Value (T::*getter)() const, void (T::*setter)(const Value &value)      ) { ClassBase::property(name, static_cast<getter_callback_1>(getter), static_cast<setter_callback_0>(setter)); return *this; }
    Class<T> &property(const char *name, Value (T::*getter)()      , void (T::*setter)(const Value &value) const) { ClassBase::property(name, static_cast<getter_callback_0>(getter), static_cast<setter_callback_1>(setter)); return *this; }
    Class<T> &property(const char *name, Value (T::*getter)() const, void (T::*setter)(const Value &value) const) { ClassBase::property(name, static_cast<getter_callback_1>(getter), static_cast<setter_callback_1>(setter)); return *this; }

    /**
     *  Add a PHP interface to the class
     * 
     *  Note that the interface that you supply must already exist! Therefore
     *  you can only supply interfaces that you created in your own extension.
     * 
     *  @param  interface   Interface object
     *  @return Class       Same object to allow chaining
     */
    Class<T> &implements(const Interface &interface) { ClassBase::implements(interface); return *this; }
    
    /**
     *  Add a base class
     * 
     *  Because PHP does not allow multiple inheritance, you can only add one
     *  base class. If you call this method more than once, the earlier base
     *  class is overridden.
     * 
     *  The base class that you supply must already be registered. And because
     *  your extension is most likely registered before any user space PHP scripts
     *  run, you can only specify classes that you created in your own extension.
     * 
     *  @param  base        Php::Class object
     *  @return Class       Same object to allow chaining
     */
    template<typename CLASS>
    Class<T> &extends(const Class<CLASS> &base) { ClassBase::extends(base); return *this; }
    
private:
    /**
     *  Method to create the object if it is default constructable
     *  @param  orig
     *  @return Base*
     */
    template <typename X = T>
    typename std::enable_if<std::is_default_constructible<X>::value, Base*>::type
    static maybeConstruct()
    {
        // create a new instance
        return new X();
    }

    /**
     *  Method to create the object if it is not default constructable
     *  @param  orig
     *  @return Base*
     */
    template <typename X = T>
    typename std::enable_if<!std::is_default_constructible<X>::value, Base*>::type
    static maybeConstruct()
    {
        // create empty instance
        return nullptr;
    }

    /**
     *  Construct a new instance of the object
     *  @return Base
     */
    virtual Base* construct() const override
    {
        // construct an instance
        return maybeConstruct<T>();
    }
    
    /**
     *  Method to clone the object if it is copy constructable
     *  @param  orig
     *  @return Base*
     */
    template <typename X = T>
    typename std::enable_if<std::is_copy_constructible<X>::value, Base*>::type
    static maybeClone(X *orig)
    {
        // create a new instance
        return new X(*orig);
    }

    /**
     *  Method to clone the object if it is not copy constructable
     *  @param  orig
     *  @return Base*
     */
    template <typename X = T>
    typename std::enable_if<!std::is_copy_constructible<X>::value, Base*>::type
    static maybeClone(X *orig)
    {
        // impossible return null
        return nullptr;
    }

    /**
     *  Is this a clonable class?
     *  @return bool
     */
    virtual bool clonable() const 
    {
        return std::is_copy_constructible<T>::value;
    }
    
    /**
     *  Construct a clone
     *  @param  orig
     *  @return Base
     */
    virtual Base *clone(Base *orig) const override
    {
        // maybe clone it (if the class has a copy constructor)
        return maybeClone<T>((T*)orig);
    }
    
    /**
     *  Is this class traversable?
     *  @return bool
     */
    virtual bool traversable() const override
    {
        // check if the templated class overrides from the Traversable class
        return std::is_base_of<Traversable,T>::value;
    }

    /**
     *  Is this a serializable class?
     *  @return bool
     */
    virtual bool serializable() const override
    {
        // check if the templated class overrides from the Serializable class
        return std::is_base_of<Serializable,T>::value;
    }

    /**
     *  Call the __clone method
     *  @param  base
     */
    virtual void callClone(Base *base) const 
    {
        // cast to the user object
        T *object = (T *)base;
        
        // call the method on the base object
        return object->__clone();
    }

    /**
     *  Call the __destruct method
     *  @param  base
     */
    virtual void callDestruct(Base *base) const
    {
        // cast to the user object
        T *object = (T *)base;
        
        // call the method on the base object
        return object->__destruct();
    }

    /**
     *  Call a method
     *  @param  base        Object to call on
     *  @param  name        Name of the method
     *  @param  params
     *  @return Value
     */
    virtual Value callCall(Base *base, const char *name, Parameters &params) const override
    {
        // cast to the user object
        T *object = (T *)base;
        
        // call the method on the base object
        return object->__call(name, params);
    }

    /**
     *  SFINAE test to check if the __callStatic method is defined
     * 
     *  This type trait checks if the __callStatic method is defined in class T
     * 
     *  @see http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
     */
    template <typename X>
    class HasCallStatic
    {
        typedef char one;
        typedef long two;

        template <typename C> static one test( decltype(&C::__callStatic) ) ;
        template <typename C> static two test(...);

    public:
        static const bool value = sizeof(test<X>(0)) == sizeof(char);
    };

    /**
     *  Function that only exists if the class T has a __callStatic method
     *  @param  name        Name of the function
     *  @param  params      Parameters passed to the function
     *  @return Value
     */
    template<typename X>
    typename std::enable_if<HasCallStatic<X>::value, Value >::type
    static maybeCallStatic(const char *name, Parameters &params)
    {
        // call the __callStatic() function
        return X::__callStatic(name, params);
    }

    /**
     *  Function that only exists if the class T does not have a __callStatic method
     *  @param  name        Name of the function
     *  @param  params      Parameters passed to the function
     *  @return Value
     */
    template<typename X>
    typename std::enable_if<!HasCallStatic<X>::value, Value >::type
    static maybeCallStatic(const char *name, Parameters &params)
    {
        // this is not implemented
        notImplemented();
        
        // unreachable
        return nullptr;
    }

    /**
     *  Call a the __callStatic() function
     *  @param  name        Name of the function
     *  @param  params      Parameters passed to the function
     *  @return Value
     */
    virtual Value callCallStatic(const char *name, Parameters &params) const override
    {
        return maybeCallStatic<T>(name, params);
    }

    /**
     *  Call the __invoke() method
     *  @param  base        Object to call it on
     *  @param  params      Parameters to pass
     *  @return Value
     */
    virtual Value callInvoke(Base *object, Parameters &params) const override
    {
        // cast to actual object
        T *obj = (T *)object;
        
        // pass on
        return obj->__invoke(params);
    }

    /**
     *  Cast to string function
     *  @param  base
     *  @return Value
     */
    virtual Value callToString(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return Value(obj->__toString()).setType(Type::String);
    }
    
    /**
     *  Cast to integer function
     *  @param  base
     *  @return Value
     */
    virtual Value callToInteger(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return Value(obj->__toInteger()).setType(Type::Numeric);
    }
    
    /**
     *  Cast to float function
     *  @param  base
     *  @return Value
     */
    virtual Value callToFloat(Base *base) const override 
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return Value(obj->__toFloat()).setType(Type::Float);
    }

    /**
     *  Cast to bool function
     *  @param  base
     *  @return Value
     */
    virtual Value callToBool(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return Value(obj->__toBool()).setType(Type::Bool);
    }

    /**
     *  Function to retrieve a property
     *  @param  base
     *  @param  name
     *  @param  value
     *  @return Value
     */
    virtual Value callGet(Base *base, const Value &name) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__get(name);
    }
    
    /**
     *  Function to set/overwrite a property
     *  @param  base
     *  @param  name
     *  @param  value
     */
    virtual void callSet(Base *base, const Value &name, const Value &value) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        obj->__set(name, value);
    }
        
    /**
     *  Function to remove a property
     *  @param  base
     *  @param  name
     */
    virtual void callUnset(Base *base, const Value &name) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        obj->__unset(name);
    }

    /**
     *  Function to check if a property is set
     *  @param  base
     *  @param  name
     *  @return bool
     */
    virtual bool callIsset(Base *base, const Value &name) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__isset(name);
    }

    /**
     *  Compare two objects
     *  @param  object1
     *  @param  object2
     *  @return int
     */
    virtual int callCompare(Base *object1, Base *object2) const override
    {
        // cast to the actual implementation type
        T *t1 = (T *)object1;
        T *t2 = (T *)object2;
        
        // compare the two objects
        return t1->__compare(*t2);
    }

    /**
     *  Namespaces and the function have access to the private base class, 
     *  so that the classes can be registered (the function object needs
     *  this to register the Functor class).
     */
    friend class Namespace;
    friend class Function;
    
    /**
     *  All Php::Class<AnyThing> also need access to the base class to
     *  register this class as base class.
     */
    template<typename ANYTHING> friend class Class;
    
};

/**
 *  End of namespace
 */
}

