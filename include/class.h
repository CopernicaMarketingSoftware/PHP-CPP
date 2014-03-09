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
    virtual Base* construct() override
    {
        // construct an instance
        return new T();
    }
    
    /**
     *  Construct a clone
     *  @param  orig
     *  @return Base
     */
    virtual Base *clone(Base *orig) override
    {
        // cast to the original object
        T *t = (T *)orig;
        
        // construct a new base by calling the copy constructor
        return new T(*t);
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

