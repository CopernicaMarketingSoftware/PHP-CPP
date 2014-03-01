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
 *  Forward declarations
 */
struct _zend_class_entry;

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
    Class(const char *name) : ClassBase(name) {}
    
    // @todo add copy and move constructor

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
    void add(const char *name, void(T::*method)(),                   int flags = Public, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_0>(method), flags,  args); }
    void add(const char *name, void(T::*method)(Parameters &params), int flags = Public, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_1>(method), flags,  args); }
    void add(const char *name, bool(T::*method)(),                   int flags = Public, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_2>(method), flags,  args); }
    void add(const char *name, bool(T::*method)(Parameters &params), int flags = Public, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_3>(method), flags,  args); }
    void add(const char *name, void(T::*method)(),                                       const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_0>(method), Public, args); }
    void add(const char *name, void(T::*method)(Parameters &params),                     const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_1>(method), Public, args); }
    void add(const char *name, bool(T::*method)(),                                       const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_2>(method), Public, args); }
    void add(const char *name, bool(T::*method)(Parameters &params),                     const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_3>(method), Public, args); }

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
    void add(const char *name, std::nullptr_t value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, uint64_t value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, uint32_t value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, uint16_t value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, char value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, const char *value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, const std::string &value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, bool value, int flags = Public) { ClassBase::add(name, value, flags); }
    void add(const char *name, double value, int flags = Public) { ClassBase::add(name, value, flags); }
     
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
     *  Namespaces have access to the private base class
     */
    friend class Namespace;
};

/**
 *  End of namespace
 */
}

