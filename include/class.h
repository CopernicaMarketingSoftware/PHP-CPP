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
 *  Zend/SPL interfaces that we support
 */
//extern struct _zend_class_entry *zend_ce_traversable;
//extern struct _zend_class_entry *zend_ce_aggregate;
//extern struct _zend_class_entry *zend_ce_iterator;
extern struct _zend_class_entry *zend_ce_arrayaccess;
//extern struct _zend_class_entry *zend_ce_serializable;

/**
 *  SFINAE test to check if the __callStatic method is defined
 *  @see http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
 */
template <typename T>
class HasCallStatic
{
    typedef char one;
    typedef long two;

    template <typename C> static one test( decltype(&C::__callStatic) ) ;
    template <typename C> static two test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

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
     *  The flags can be a combination of Php::Final and Php::Abstract. If no
     *  flags are set, a regular public class will be formed.
     * 
     *  @param  name        Name of the class
     */
    Class(const char *name) : ClassBase(name)
    {
        // check for special classes, and register the interface if it does
        // register the interface (we register a pointer-to-a-pointer here,
        // because when this code runs (during the get_module() call), the 
        // interfaces are not yet initialized by the zend engine, this only
        // happens later when the all classes are registered (after the
        // get_module() call)
//        if (std::is_base_of<ArrayAccess, T>::value) interface(&zend_ce_arrayaccess);
    }
    
    /**
     *  Copy constructor
     *  @param  that
     */
    Class(const Class<T> &that) : ClassBase(that) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Class(Class<T> &&that) : ClassBase(std::move(that)) {}

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
     */
    void method(const char *name, void  (T::*method)(),                         int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_0>(method), flags,  args); }
    void method(const char *name, void  (T::*method)(Parameters &params),       int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_1>(method), flags,  args); }
    void method(const char *name, Value (T::*method)(),                         int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_2>(method), flags,  args); }
    void method(const char *name, Value (T::*method)(Parameters &params),       int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_3>(method), flags,  args); }
    void method(const char *name, void  (T::*method)(),                                    const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_0>(method), Public, args); }
    void method(const char *name, void  (T::*method)(Parameters &params),                  const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_1>(method), Public, args); }
    void method(const char *name, Value (T::*method)(),                                    const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_2>(method), Public, args); }
    void method(const char *name, Value (T::*method)(Parameters &params),                  const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_3>(method), Public, args); }
    void method(const char *name, void  (T::*method)()                   const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_4>(method), flags,  args); }
    void method(const char *name, void  (T::*method)(Parameters &params) const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_5>(method), flags,  args); }
    void method(const char *name, Value (T::*method)()                   const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_6>(method), flags,  args); }
    void method(const char *name, Value (T::*method)(Parameters &params) const, int flags, const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_7>(method), flags,  args); }
    void method(const char *name, void  (T::*method)()                   const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_4>(method), Public, args); }
    void method(const char *name, void  (T::*method)(Parameters &params) const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_5>(method), Public, args); }
    void method(const char *name, Value (T::*method)()                   const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_6>(method), Public, args); }
    void method(const char *name, Value (T::*method)(Parameters &params) const,            const Arguments &args = {}) { ClassBase::method(name, static_cast<method_callback_7>(method), Public, args); }

    /**
     *  Add a static method to a class
     * 
     *  In C++ a static method is just a plain function, that only at compile
     *  time has access to the private variables. You can therefore also supply
     *  global functions as static method, and real static methods (that do not
     *  even have to come from the same class.
     * 
     *  In PHP scripts, the function will only be callable as real static method
     * 
     *  @param  name        Name of the method
     *  @param  method      The actual method
     *  @param  flags       Optional flags
     *  @param  args        Argument descriptions
     */
    void method(const char *name, const native_callback_0 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); }
    void method(const char *name, const native_callback_1 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); }
    void method(const char *name, const native_callback_2 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); }
    void method(const char *name, const native_callback_3 &function, int flags, const Arguments &args = {}) { ClassBase::method(name, function, flags,  args); }
    void method(const char *name, const native_callback_0 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); }
    void method(const char *name, const native_callback_1 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); }
    void method(const char *name, const native_callback_2 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); }
    void method(const char *name, const native_callback_3 &function,            const Arguments &args = {}) { ClassBase::method(name, function, Public, args); }

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
     */
    void method(const char *name, int flags, const Arguments &args = {}) { ClassBase::method(name, flags  | Abstract, args); }
    void method(const char *name,            const Arguments &args = {}) { ClassBase::method(name, Public | Abstract, args); }

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
    void property(const char *name, std::nullptr_t value,     int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, uint64_t value,           int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, uint32_t value,           int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, uint16_t value,           int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, char value,               int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, const char *value,        int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, const std::string &value, int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, bool value,               int flags = Public) { ClassBase::property(name, value, flags); }
    void property(const char *name, double value,             int flags = Public) { ClassBase::property(name, value, flags); }
     
protected:
    /**
     *  Protected constructor
     *  @param  name
     *  @param  flags
     */
    Class(const char *name, int flags) : ClassBase(name, flags) {}

private:
    /**
     *  Construct a new instance of the object
     *  @return Base
     */
    virtual Base* construct() const override
    {
        // construct an instance
        return new T();
    }
    
    /**
     *  Construct a clone
     *  @param  orig
     *  @return Base
     */
    virtual Base *clone(Base *orig) const override
    {
        // cast to the original object
        T *t = (T *)orig;
        
        // construct a new base by calling the copy constructor
        return new T(*t);
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
     *  Call a method
     *  @param  base        Object to call on
     *  @param  name        Name of the method
     *  @param  params
     *  @return Value
     */
    virtual Value call(Base *base, const char *name, Parameters &params) const override
    {
        // cast to the user object
        T *object = (T *)base;
        
        // call the method on the base object
        return base->__call(name, params);
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
    virtual Value callStatic(const char *name, Parameters &params) const override
    {
        return maybeCallStatic<T>(name, params);
    }

    /**
     *  Call the __invoke() method
     *  @param  base        Object to call it on
     *  @param  params      Parameters to pass
     *  @return Value
     */
    virtual Value invoke(Base *object, Parameters &params) const override
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
    virtual Value toString(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__toString().setType(Type::String);
    }
    
    /**
     *  Cast to integer function
     *  @param  base
     *  @return Value
     */
    virtual Value toInteger(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__toInteger().setType(Type::Numeric);
    }
    
    /**
     *  Cast to float function
     *  @param  base
     *  @return Value
     */
    virtual Value toFloat(Base *base) const override 
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__toFloat().setType(Type::Float);
    }

    /**
     *  Cast to bool function
     *  @param  base
     *  @return Value
     */
    virtual Value toBool(Base *base) const override
    {
        // cast to actual object
        T *obj = (T *)base;
        
        // pass on
        return obj->__toBool().setType(Type::Bool);
    }

    /**
     *  Function to retrieve a property
     *  @param  base
     *  @param  name
     *  @param  value
     *  @return Value
     */
    virtual Value get(Base *base, const Value &name) const override
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
    virtual void set(Base *base, const Value &name, const Value &value) const override
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
    virtual void unset(Base *base, const Value &name) const override
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
    virtual int compare(Base *object1, Base *object2) const override
    {
        // cast to the actual implementation type
        T *t1 = (T *)object1;
        T *t2 = (T *)object2;
        
        // compare the two objects
        return t1->__compare(*t2);
    }

    /**
     *  Namespaces have access to the private base class
     */
    friend class Namespace;
};

/**
 *  End of namespace
 */
}

