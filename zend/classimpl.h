/**
 *  ClassImpl.h
 *
 *  Base implementation class that stores all methods and properties that
 *  exist for a class.
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
class ClassImpl
{
private:
    /**
     *  Pointer to the actual Php::Class<X> that is created in the extension
     *  @var    ClassBase
     */
    ClassBase *_base = nullptr;

    /**
     *  Name of the class
     *  @var    string
     */
    std::string _name;

    /**
     *  The comment for reflexion, with a stored pointer to ourselves
     *  @var    char*
     */
    char *_comment = nullptr;

    /**
     *  The class type (this can be values like Php::Abstract and Php::Final)
     *  @var    ClassType
     */
    ClassType _type = ClassType::Regular;

    /** 
     *  The class entry
     *  @var    zend_class_entry
     */
    zend_class_entry *_entry = nullptr;

    /**
     *  Pointer to the entries
     *  @var    zend_function_entry[]
     */
    zend_function_entry *_entries = nullptr;
    
    /**
     *  All class methods
     *  @var    std::list
     */
    std::list<std::shared_ptr<Method>> _methods;
    
    /**
     *  All class members (class properties)
     *  @var    std::list
     */
    std::list<std::shared_ptr<Member>> _members;
    
    /**
     *  Map of dynamically accessible properties
     *  @var    std::map
     */
    std::map<std::string,std::shared_ptr<Property>> _properties;

    /**
     *  Interfaces that are implemented
     *  @var    std::list
     */
    std::list<std::shared_ptr<ClassImpl>> _interfaces;
    
    /**
     *  The parent/base class
     *  @var    std::shared_ptr
     */
    std::shared_ptr<ClassImpl> _parent;
    
    /**
     *  The object handlers for instances of this class
     *  @var    zend_object_handlers
     */
    zend_object_handlers _handlers;
    
    /**
     *  Are the handlers already initialized?
     *  @var    bool
     */
    bool _initialized = false;


    /**
     *  Retrieve an array of zend_function_entry objects that hold the 
     *  properties for each method. This method is called at extension
     *  startup time to register all methods.
     * 
     *  @param  classname       The class name
     *  @return zend_function_entry[]
     */
    const zend_function_entry *entries();

    /**
     *  Helper method to turn a property into a zval
     *  @param  value
     *  @param  type
     *  @return Value
     */
    static zval *toZval(Value &&value, int type);

public:
    /**
     *  Constructor
     *  @param  name            Class name
     *  @param  type            Class type
     */
    ClassImpl(const char *name, ClassType type) : _name(name), _type(type) {}

    /**
     *  No copying or moving
     *  @param  that
     */
    ClassImpl(const ClassImpl &that) = delete;
    ClassImpl(ClassImpl &&that) = delete;

    /**
     *  Destructor
     */
    virtual ~ClassImpl();

    /**
     *  Retrieve the class name
     *  @return std::string
     */
    const std::string &name() const
    {
        return _name;
    }

    /**
     *  Initialize the class, given its name
     * 
     *  The module functions are registered on module startup, but classes are
     *  initialized afterwards. The Zend engine is a strange thing. Nevertheless,
     *  this means that this method is called after the module is already available.
     *  This function will inform the Zend engine about the existence of the
     *  class.
     * 
     *  @param  base        The extension C++ class 
     *  @param  ns          Namespace name
     *  @param  tsrm_ls
     */
    void initialize(ClassBase *base, const std::string &ns TSRMLS_DC);

    /**
     *  Static member functions to create or clone objects based on this class
     *  @param  entry                   Pointer to class information
     *  @param  val                     The object to be cloned
     *  @param  tsrm_ls
     *  @return zend_object_value       Object info
     */
    static zend_object_value createObject(zend_class_entry *entry TSRMLS_DC);
    static zend_object_value cloneObject(zval *val TSRMLS_DC);
    static void destructObject(zend_object *object, unsigned int handle TSRMLS_DC);
    static void freeObject(zend_object *object TSRMLS_DC);

    /**
     *  Static member function that get called when a method or object is called
     *  @param  ht                      ??
     *  @param  return_value            Zval holding the variable to store the return value in
     *  @param  return_value_ptr        Pointer to the same zval
     *  @param  this_ptr                Object being called
     *  @param  return_value_used       Is the return value used or not?
     *  @param  tsrm_ls
     */
    static void callMethod(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC);
    static void callInvoke(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC);

    /**
     *  Function that is used to count the number of elements in the object
     *  If the user has implemented the Countable interface, this method will 
     *  call the count() method
     *  @param  val
     *  @param  count
     *  @param  tsrm_ls
     *  @return int
     */
    static int countElements(zval *object, long *count TSRMLS_DC);

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  value           The new value
     *  @param  type            The type of the variable???
     *  @param  check_empty     ????
     *  @return zval
     */
    static zval *readDimension(zval *object, zval *offset, int type TSRMLS_DC);
    static void writeDimension(zval *object, zval *offset, zval *value TSRMLS_DC);
    static int  hasDimension(zval *object, zval *offset, int check_empty TSRMLS_DC);
    static void unsetDimension(zval *object, zval *offset TSRMLS_DC);

    /**
     *  Retrieve pointer to our own object handlers
     *  @return zend_object_handlers
     */
    zend_object_handlers *objectHandlers();

    /**
     *  Alternative way to retrieve object handlers, given a class entry
     *  @param  entry
     *  @return zend_object_handlers
     */
    static zend_object_handlers *objectHandlers(zend_class_entry *entry);
    
    /**
     *  Function to create a new iterator to iterate over an object
     *  @param  entry                   The class entry
     *  @param  object                  The object to iterate over
     *  @param  by_ref                  ?????
     *  @param  tsrm_ls
     *  @return zend_object_iterator*   Pointer to the iterator
     */
    static zend_object_iterator *getIterator(zend_class_entry *entry, zval *object, int by_ref TSRMLS_DC);

    /**
     *  Function that is called when a property is being read
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  type            The type of the variable???
     *  @param  key             ???
     *  @param  tsrm_ls
     *  @return zval
     */
#if PHP_VERSION_ID >= 50400    
    static zval *readProperty(zval *object, zval *name, int type, const zend_literal *key TSRMLS_DC);
#else
    static zval *readProperty(zval *object, zval *name, int type TSRMLS_DC);
#endif

    /**
     *  Function that is called when a property is set / updated
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property
     *  @param  value           The new value
     *  @param  key             ???
     *  @param  tsrm_ls
     *  @return zval
     */
#if PHP_VERSION_ID >= 50400    
    static void writeProperty(zval *object, zval *name, zval *value, const zend_literal *key TSRMLS_DC);
#else
    static void writeProperty(zval *object, zval *name, zval *value TSRMLS_DC);
#endif

    /**
     *  Function that is called to check whether a certain property is set
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property to check
     *  @param  has_set_exists  See above
     *  @param  tsrm_ls
     *  @return bool
     */
#if PHP_VERSION_ID >= 50400    
    static int hasProperty(zval *object, zval *name, int has_set_exists, const zend_literal *key TSRMLS_DC);
#else
    static int hasProperty(zval *object, zval *name, int has_set_exists TSRMLS_DC);
#endif

    /**
     *  Function that is called when a property is removed from the project
     *  @param  object          The object on which it is called
     *  @param  member          The member to remove
     *  @param  tsrm_ls
     */
#if PHP_VERSION_ID >= 50400    
    static void unsetProperty(zval *object, zval *member, const zend_literal *key TSRMLS_DC);
#else
    static void unsetProperty(zval *object, zval *member TSRMLS_DC);
#endif

    /**
     *  Method that returns information about the function signature of a undefined method
     *  @param  object_ptr
     *  @param  method
     *  @param  method_len
     *  @param  key
     *  @param  tsrm_ls
     *  @return zend_function
     */
#if PHP_VERSION_ID >= 50400    
    static zend_function *getMethod(zval **object_ptr, char *method, int method_len, const zend_literal *key TSRMLS_DC);
#else
    static zend_function *getMethod(zval **object_ptr, char *method, int method_len TSRMLS_DC);
#endif

    /**
     *  Method that returns information about the function signature of an undefined static method
     *  @param  object_ptr
     *  @param  method
     *  @param  method_len
     *  @param  key
     *  @param  tsrm_ls
     *  @return zend_function
     */
    static zend_function *getStaticMethod(zend_class_entry *entry, char* method, int method_len TSRMLS_DC);

    /**
     *  Method that returns information about the __invoke() method
     *  @param  object
     *  @param  entry
     *  @param  func
     *  @param  object_ptr
     *  @param  tsrm_ls
     *  @return int
     */
    static int getClosure(zval *object, zend_class_entry **entry, zend_function **func, zval **object_ptr TSRMLS_DC);

    /**
     *  Function to cast the object to a different type
     *  @param  object
     *  @param  retval
     *  @param  type
     *  @param  tsrm_ls
     *  @return int
     */
    static int cast(zval *object, zval *retval, int type TSRMLS_DC);

    /**
     *  Function to compare two objects
     *  @param  object1
     *  @param  object2
     *  @param  tsrm_ls
     *  @return int
     */
    static int compare(zval *object1, zval *object2 TSRMLS_DC);

    /**
     *  Methods that are called to serialize/unserialize an object
     *  @param  object      The object to be serialized
     *  @param  entry       The class entry to which the object belongs
     *  @param  buffer      Buffer in which to store the data
     *  @param  buf_len     Size of the bufffer
     *  @param  data        Structure describing the serialize/unserialize data
     *  @param  tsrm_ls
     *  @return int
     */
    static int serialize(zval *object, unsigned char **buffer, unsigned int *buf_len, zend_serialize_data *data TSRMLS_DC);
    static int unserialize(zval **object, zend_class_entry *entry, const unsigned char *buffer, unsigned int buf_len, zend_unserialize_data *data TSRMLS_DC);
    
    /**
     *  Add a method to the class
     *  zend_serialize_data     
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
    void method(const char *name, const method_callback_0 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_1 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_2 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_3 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_4 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_5 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_6 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }
    void method(const char *name, const method_callback_7 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args, return_ref)); }

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
    void method(const char *name, const native_callback_0 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args, return_ref)); }
    void method(const char *name, const native_callback_1 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args, return_ref)); }
    void method(const char *name, const native_callback_2 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args, return_ref)); }
    void method(const char *name, const native_callback_3 &method, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args, return_ref)); }

    /**
     *  Add an abstract method to the class
     * 
     *  @param  name        Name of the method
     *  @param  flags       Optional flags (like public or protected)
     *  @param  args        Description of the supported arguments
     *  @param  return_ref  Return reference or not
     */
    void method(const char *name, int flags=0, const Arguments &args = {}, bool return_ref = false) { _methods.push_back(std::make_shared<Method>(name, (flags & (MethodModifiers | Static)) | Abstract , args, return_ref)); }

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
    void property(const char *name, std::nullptr_t value, int flags = Php::Public)      { _members.push_back(std::make_shared<NullMember>   (name,                        flags & PropertyModifiers)); }
    void property(const char *name, int16_t value, int flags = Php::Public)             { _members.push_back(std::make_shared<NumericMember>(name,  value,                flags & PropertyModifiers)); }
    void property(const char *name, int32_t value, int flags = Php::Public)             { _members.push_back(std::make_shared<NumericMember>(name,  value,                flags & PropertyModifiers)); }
    void property(const char *name, int64_t value, int flags = Php::Public)             { _members.push_back(std::make_shared<NumericMember>(name,  value,                flags & PropertyModifiers)); }
    void property(const char *name, bool value, int flags = Php::Public)                { _members.push_back(std::make_shared<BoolMember>   (name,  value,                flags & PropertyModifiers)); }
    void property(const char *name, char value, int flags = Php::Public)                { _members.push_back(std::make_shared<StringMember> (name, &value,             1, flags & PropertyModifiers)); }
    void property(const char *name, const std::string &value, int flags = Php::Public)  { _members.push_back(std::make_shared<StringMember> (name,  value,                flags & PropertyModifiers)); }
    void property(const char *name, const char *value, int flags = Php::Public)         { _members.push_back(std::make_shared<StringMember> (name,  value, ::strlen(value), flags & PropertyModifiers)); }
    void property(const char *name, double value, int flags = Php::Public)              { _members.push_back(std::make_shared<FloatMember>  (name,  value,                flags & PropertyModifiers)); }

    /**
     *  Set property with callbacks
     *  @param  name        Name of the property
     *  @param  getter      Getter method
     *  @param  setter      Setter method
     */
    void property(const char *name, const getter_callback_0 &getter)                                    { _properties[name] = std::make_shared<Property>(getter); }
    void property(const char *name, const getter_callback_1 &getter)                                    { _properties[name] = std::make_shared<Property>(getter); }
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_0 &setter)   { _properties[name] = std::make_shared<Property>(getter,setter); }
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_0 &setter)   { _properties[name] = std::make_shared<Property>(getter,setter); }
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_1 &setter)   { _properties[name] = std::make_shared<Property>(getter,setter); }
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_1 &setter)   { _properties[name] = std::make_shared<Property>(getter,setter); }
    
    /**
     *  Add an interface that is implemented
     *  @param  interface   The interface that is implemented
     */
    void implements(const std::shared_ptr<ClassImpl> &interface) { _interfaces.push_back(interface); }

    /**
     *  Set the base class
     *  @param  base        The base class
     */
    void extends(const std::shared_ptr<ClassImpl> &base) { _parent = base; }

};

/**
 *  End namespace
 */
}

