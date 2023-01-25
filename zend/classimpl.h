/**
 *  ClassImpl.h
 *
 *  Base implementation class that stores all methods and properties that
 *  exist for a class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 - 2019 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

#if PHP_VERSION_ID >= 70400
#	define PHP_WRITE_PROP_HANDLER_TYPE zval *
#else
#	define PHP_WRITE_PROP_HANDLER_TYPE void
#endif

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
     *  Memory allocated by this object to hide a pointer
     *  @var    zend_string*
     */
    zend_string *_self = nullptr;

    /**
     *  Retrieve an array of zend_function_entry objects that hold the
     *  properties for each method. This method is called at extension
     *  startup time to register all methods.
     *
     *  @return zend_function_entry[]
     */
    const zend_function_entry *entries();

    /**
     *  Helper method to check if a function is registered for this instance
     *  @param name         name of the function to check for
     *  @return bool        Wether the function exists or not
     */
    bool hasMethod(const char* name) const;

    /**
     *  Helper method to turn a property into a zval
     *
     *  @param  value   The value to convert to a zval
     *  @param  type    The type of operation (read or write)
     *  @param  rv      Pointer to where to store the data
     *  @return The result (same as the ptr input)
     */
    static zval *toZval(Value &&value, int type, zval *rv);

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
     *  The class-entry
     *  @return zend_class_entry
     */
    struct _zend_class_entry *entry() const
    {
        return _entry;
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
     *  @return zend_class_entry
     */
    struct _zend_class_entry *initialize(ClassBase *base, const std::string &ns);

    /**
     *  Static member functions to create or clone objects based on this class
     *  @param  entry                   Pointer to class information
     *  @param  val                     The object to be cloned
     *  @return zend_object             Object info
     */
    static zend_object *createObject(zend_class_entry *entry);
#if PHP_VERSION_ID < 80000
    static zend_object *cloneObject(zval *val);
#else
    static zend_object *cloneObject(zend_object *val);
#endif
    static void destructObject(zend_object *object);
    static void freeObject(zend_object *object);

    /**
     *  Static member function that get called when a method or object is called
     *  @param  ht                      ??
     *  @param  return_value            Zval holding the variable to store the return value in
     *  @param  return_value_ptr        Pointer to the same zval
     *  @param  this_ptr                Object being called
     *  @param  return_value_used       Is the return value used or not?
     */
    static void callMethod(zend_execute_data *execute_data, zval *return_value);
    static void callInvoke(zend_execute_data *execute_data, zval *return_value);

    /**
     *  Function that is used to count the number of elements in the object
     *  If the user has implemented the Countable interface, this method will
     *  call the count() method
     *  @param  val
     *  @param  count
     *  @return int
     */
#if PHP_VERSION_ID < 80000
    static int countElements(zval *object, zend_long *count);
#elif PHP_VERSION_ID < 80200
    static int countElements(zend_object *object, zend_long *count);
#else
    static zend_result countElements(zend_object *object, zend_long *count);
#endif

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  value           The new value
     *  @param  type            The type of the variable???
     *  @param  rv              Pointer to where to store the data
     *  @param  check_empty     ????
     *  @return zval
     */
#if PHP_VERSION_ID < 80000
    static zval *readDimension(zval *object, zval *offset, int type, zval *rv);
    static void writeDimension(zval *object, zval *offset, zval *value);
    static int  hasDimension(zval *object, zval *offset, int check_empty);
    static void unsetDimension(zval *object, zval *offset);
#else
    static zval *readDimension(zend_object *object, zval *offset, int type, zval *rv);
    static void writeDimension(zend_object *object, zval *offset, zval *value);
    static int  hasDimension(zend_object *object, zval *offset, int check_empty);
    static void unsetDimension(zend_object *object, zval *offset);
#endif

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
     *  @return zend_object_iterator*   Pointer to the iterator
     */
    static zend_object_iterator *getIterator(zend_class_entry *entry, zval *object, int by_ref);

    /**
     *  Function that is called when a property is being read
     *
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  type            The type of the variable???
     *  @param  cache_slot      The cache slot used
     *  @param  rv              Pointer to where to store the data
     *  @return zval
     */
#if PHP_VERSION_ID < 80000
    static zval *readProperty(zval *object, zval *name, int type, void **cache_slot, zval *rv);
#else
    static zval *readProperty(zend_object *object, zend_string *name, int type, void **cache_slot, zval *rv);
#endif

    /**
     *  Function that is called when a property is set / updated
     *
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property
     *  @param  value           The new value
     *  @param  cache_slot      The cache slot used
     *  @return zval*
     */
#if PHP_VERSION_ID < 80000
    static PHP_WRITE_PROP_HANDLER_TYPE writeProperty(zval *object, zval *name, zval *value, void **cache_slot);
#else
    static PHP_WRITE_PROP_HANDLER_TYPE writeProperty(zend_object *object, zend_string *name, zval *value, void **cache_slot);
#endif

    /**
     *  Function that is called to check whether a certain property is set
     *
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property to check
     *  @param  has_set_exists  See above
     *  @param  cache_slot      The cache slot used
     *  @return bool
     */
#if PHP_VERSION_ID < 80000
    static int hasProperty(zval *object, zval *name, int has_set_exists, void **cache_slot);
#else
    static int hasProperty(zend_object *object, zend_string *name, int has_set_exists, void **cache_slot);
#endif

    /**
     *  Function that is called when a property is removed from the project
     *
     *  @param  object          The object on which it is called
     *  @param  member          The member to remove
     *  @param  cache_slot      The cache slot used
     */
#if PHP_VERSION_ID < 80000
    static void unsetProperty(zval *object, zval *member, void **cache_slot);
#else
    static void unsetProperty(zend_object *object, zend_string *member, void **cache_slot);
#endif

    /**
     *  Method that returns information about the function signature of a undefined method
     *
     *  @param  object      Pointer to the object from which we want to retrieve the member function
     *  @param  method      The method that we want information about
     *  @param  key         ???
     *  @return zend_function
     */
    static zend_function *getMethod(zend_object **object, zend_string *method, const zval *key);

    /**
     *  Method that returns information about the function signature of an undefined static method
     *
     *  @param  entry       The class entry to find the static function in
     *  @param  method      The method that we want information about
     *  @param  key         ???
     *  @return zend_function
     */
    static zend_function *getStaticMethod(zend_class_entry *entry, zend_string *method);

    /**
     *  Method that returns information about the __invoke() method
     *  @param  object      The object on which the method is called
     *  @param  entry       Class entry holding class properties
     *  @param  func        Function to be filled
     *  @param  object_ptr  To be filled with the object on which the method is to be called
     *  @return int
     */
#if PHP_VERSION_ID < 80000
    static int getClosure(zval *object, zend_class_entry **entry, zend_function **func, zend_object **object_ptr);
#elif PHP_VERSION_ID < 80200
    static int getClosure(zend_object *object, zend_class_entry **entry, zend_function **func, zend_object **object_ptr, zend_bool check_only);
#else
    static zend_result getClosure(zend_object *object, zend_class_entry **entry, zend_function **func, zend_object **object_ptr, zend_bool check_only);
#endif

    /**
     *  Function to cast the object to a different type
     *  @param  object
     *  @param  retval
     *  @param  type
     *  @return int
     */
#if PHP_VERSION_ID < 80000
    static int cast(zval *object, zval *retval, int type);
#elif PHP_VERSION_ID < 80200
    static int cast(zend_object *object, zval *retval, int type);
#else
    static zend_result cast(zend_object *object, zval *retval, int type);
#endif

    /**
     *  Function to compare two objects
     *  @param  object1
     *  @param  object2
     *  @return int
     */
    static int compare(zval *object1, zval *object2);

    /**
     *  Methods that are called to serialize/unserialize an object
     *  @param  object      The object to be serialized
     *  @param  entry       The class entry to which the object belongs
     *  @param  buffer      Buffer in which to store the data
     *  @param  buf_len     Size of the bufffer
     *  @param  data        Structure describing the serialize/unserialize data
     *  @return int
     */
    static int serialize(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
    static int unserialize(zval *object, zend_class_entry *entry, const unsigned char *buffer, size_t buf_len, zend_unserialize_data *data);

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
     *  @param  callback    The actual method
     *  @param  flags       Optional flags
     *  @param  args        Description of the supported arguments
     */
    void method(const char *name, ZendCallback callback, int flags = 0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, callback, flags & MethodModifiers, args)); }

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
     */
    void method(const char *name, const method_callback_0 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_1 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_2 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_3 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_4 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_5 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_6 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }
    void method(const char *name, const method_callback_7 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, flags & MethodModifiers, args)); }

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
     */
    void method(const char *name, const native_callback_0 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args)); }
    void method(const char *name, const native_callback_1 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args)); }
    void method(const char *name, const native_callback_2 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args)); }
    void method(const char *name, const native_callback_3 &method, int flags=0, const Arguments &args = {}) { _methods.push_back(std::make_shared<Method>(name, method, (flags & MethodModifiers) | Static, args)); }

    /**
     *  Add an abstract method to the class
     *
     *  @param  name        Name of the method
     *  @param  flags       Optional flags (like public or protected)
     *  @param  args        Description of the supported arguments
     */
    void method(const char *name, int flags=0, const Arguments &args = {})
    {
        // the "MethodModifiers" holds all the valid modifiers for a method: Final + Public + Protected + Private.
        // The "Static" and "Abstract" properties are also valid modifiers in this context (in fact, you would
        // expect that we could even force adding "Abstract" here, because we're adding an abstract method -- but
        // in a PHP interface the "Abstract" modifier is not allowed - even though it is of course abstract.
        // So we only _allow_ abstract here, and expect the caller to _set_ it.
        _methods.push_back(std::make_shared<Method>(name, (flags & (MethodModifiers | Static | Abstract)), args));
    }

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

