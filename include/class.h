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
     * 
     *  @todo   make sure flags are used
     */
    Class(const char *name) : ClassBase(name) {}

    /**
     *  Destructor
     */
    virtual ~Class() {}
    
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
     *  @param  property    Actual property value
     *  @param  flags       Optional flags
     */
//    void add(const char *name, const Property &property, int flags = Php::Public)
//    {
//        // @todo    something with the flags
//        _properties[name] = property;
//    }
    
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
    void add(const char *name, void(T::*method)(),                   int flags = 0, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_0>(method), flags, args); }
    void add(const char *name, void(T::*method)(Parameters &params), int flags = 0, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_1>(method), flags, args); }
    void add(const char *name, bool(T::*method)(),                   int flags = 0, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_2>(method), flags, args); }
    void add(const char *name, bool(T::*method)(Parameters &params), int flags = 0, const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_3>(method), flags, args); }
    void add(const char *name, void(T::*method)(),                                  const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_0>(method), 0,     args); }
    void add(const char *name, void(T::*method)(Parameters &params),                const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_1>(method), 0,     args); }
    void add(const char *name, bool(T::*method)(),                                  const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_2>(method), 0,     args); }
    void add(const char *name, bool(T::*method)(Parameters &params),                const Arguments &args = {}) { ClassBase::add(name, static_cast<method_callback_3>(method), 0,     args); }
     
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
     *  Extensions have access to the private base class
     */
    friend class Extension;
};

/**
 *  End of namespace
 */
}

