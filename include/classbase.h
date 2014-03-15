/**
 *  ClassBase.h
 *
 *  This is the base class of the "Class" class. This is an internal class that
 *  is used by the PHP-CPP library. But because the constructor is protected,
 *  you can not create any instances if this class yourself (and you are not
 *  supposed to do that either.
 *
 *  Further more, because this base class is a 'private' base of Class, all 
 *  features of it are normally also inaccessible.
 *
 *  In other words: it is not meant to be directly used by extension writers.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_object_value;
struct _zend_object_handlers;
struct _zend_class_entry;
struct _zend_serialize_data;
struct _zend_unserialize_data;
union _zend_function;

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
class Method;
class Member;
class Property;

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
    ClassBase(const char *classname, ClassType type) : _name(classname), _type(type) {}
    
public:
    /**
     *  Copy constructor
     *  @param  that
     */
    ClassBase(const ClassBase &that) : 
        _name(that._name), 
        _type(that._type), 
        _methods(that._methods), 
        _members(that._members), 
        _properties(that._properties),
        _entry(nullptr) {}

    /**
     *  Move constructor
     *  @param  that
     */
    ClassBase(ClassBase &&that) :
        _name(std::move(that._name)), 
        _type(that._type), 
        _methods(std::move(that._methods)), 
        _members(std::move(that._members)), 
        _properties(std::move(that._properties)),
        _entry(that._entry) 
    {
        // other entry are invalid now (not that it is used..., class objects are
        // only moved during extension setup, when the entry pointer has not yet
        // been allocated)
        that._entry = nullptr;
    }

    /**
     *  Destructor
     */
    virtual ~ClassBase();

    /**
     *  Initialize the class, given its name
     * 
     *  The module functions are registered on module startup, but classes are
     *  initialized afterwards. The Zend engine is a strange thing. Nevertheless,
     *  this means that this method is called after the module is already available.
     *  This function will inform the Zend engine about the existence of the
     *  class.
     * 
     *  @param  ns          Namespace name
     */
    void initialize(const std::string &ns);

protected:
    /**
     *  Construct a new instance of the object, or to clone the object
     *  @return Base
     */
    virtual Base* construct() const { return nullptr; }
    virtual Base *clone(Base *orig) const { return nullptr; }

    /**
     *  Methods to check if a certain interface is overridden, or a copy
     *  constructor is available
     *  @return bool
     */
    virtual bool traversable() const { return false; }
    virtual bool serializable() const { return false; }
    virtual bool clonable() const { return false; }

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
     */
    void method(const char *name, const method_callback_0 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_1 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_2 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_3 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_4 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_5 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_6 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const method_callback_7 &method, int flags=0, const Arguments &args = {});

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
    void method(const char *name, const native_callback_0 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const native_callback_1 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const native_callback_2 &method, int flags=0, const Arguments &args = {});
    void method(const char *name, const native_callback_3 &method, int flags=0, const Arguments &args = {});

    /**
     *  Add an abstract method to the class
     * 
     *  @param  name        Name of the method
     *  @param  flags       Optional flags (like public or protected)
     *  @param  args        Description of the supported arguments
     */
    void method(const char *name, int flags=0, const Arguments &args = {});

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
     *  @param  getter      Getter method
     *  @param  setter      Setter method
     */
    void property(const char *name, const getter_callback_0 &getter);
    void property(const char *name, const getter_callback_1 &getter);
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_0 &setter);
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_0 &setter);
    void property(const char *name, const getter_callback_0 &getter, const setter_callback_1 &setter);
    void property(const char *name, const getter_callback_1 &getter, const setter_callback_1 &setter);

private:
    /**
     *  Retrieve an array of zend_function_entry objects that hold the 
     *  properties for each method. This method is called at extension
     *  startup time to register all methods.
     * 
     *  @param  classname       The class name
     *  @return zend_function_entry[]
     */
    const struct _zend_function_entry *entries();

    /**
     *  Helper method to turn a property into a zval
     *  @param  value
     *  @param  type
     *  @return Value
     */
    static struct _zval_struct *toZval(Value &&value, int type);

    /**
     *  Static member functions to create or clone objects based on this class
     *  @param  entry                   Pointer to class information
     *  @param  val                     The object to be cloned
     *  @return zend_object_value       Object info
     */
    static struct _zend_object_value createObject(struct _zend_class_entry *entry);
    static struct _zend_object_value cloneObject(struct _zval_struct *val);
    static void destructObject(struct _zend_object *object, unsigned int handle);
    static void freeObject(struct _zend_object *object);

    /**
     *  Static member function that get called when a method or object is called
     *  @param  ht                      ??
     *  @param  return_value            Zval holding the variable to store the return value in
     *  @param  return_value_ptr        Pointer to the same zval
     *  @param  this_ptr                Object being called
     *  @param  return_value_used       Is the return value used or not?
     */
    static void callMethod(int ht, struct _zval_struct *return_value, struct _zval_struct **return_value_ptr, struct _zval_struct *this_ptr, int return_value_used);
    static void callInvoke(int ht, struct _zval_struct *return_value, struct _zval_struct **return_value_ptr, struct _zval_struct *this_ptr, int return_value_used);

    /**
     *  Function that is used to count the number of elements in the object
     *  If the user has implemented the Countable interface, this method will 
     *  call the count() method
     *  @param  val
     *  @param  count
     *  @return int
     */
    static int countElements(struct _zval_struct *object, long *count);

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  type            The type of the variable???
     *  @return zval
     */
    static struct _zval_struct *readDimension(struct _zval_struct *object, struct _zval_struct *offset, int type);

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  value           The new value
     *  @return zval
     */
    static void writeDimension(struct _zval_struct *object, struct _zval_struct *offset, struct _zval_struct *value);

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  member          The member to check
     *  @param  check_empty     ????
     *  @return bool
     */
    static int hasDimension(struct _zval_struct *object, struct _zval_struct *member, int check_empty);

    /**
     *  Function that is called when the object is used as an array in PHP
     *  @param  object          The object on which it is called
     *  @param  member          The member to remove
     */
    static void unsetDimension(struct _zval_struct *object, struct _zval_struct *member);

    /**
     *  Retrieve pointer to our own object handlers
     *  @return zend_object_handlers
     */
    struct _zend_object_handlers *objectHandlers();

    /**
     *  Function to create a new iterator to iterate over an object
     *  @param  entry                   The class entry
     *  @param  object                  The object to iterate over
     *  @param  by_ref                  ?????
     *  @return zend_object_iterator*   Pointer to the iterator
     */
    static struct _zend_object_iterator *getIterator(struct _zend_class_entry *entry, struct _zval_struct *object, int by_ref);

    /**
     *  Function that is called when a property is being read
     *  @param  object          The object on which it is called
     *  @param  offset          The name of the property
     *  @param  type            The type of the variable???
     *  @param  key             ???
     *  @return zval
     */
    static struct _zval_struct *readProperty(struct _zval_struct *object, struct _zval_struct *name, int type, const struct _zend_literal *key);
    static struct _zval_struct *readProperty(struct _zval_struct *object, struct _zval_struct *name, int type);

    /**
     *  Function that is called when a property is set / updated
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property
     *  @param  value           The new value
     *  @param  key             ???
     *  @return zval
     */
    static void writeProperty(struct _zval_struct *object, struct _zval_struct *name, struct _zval_struct *value, const struct _zend_literal *key);
    static void writeProperty(struct _zval_struct *object, struct _zval_struct *name, struct _zval_struct *value);

    /**
     *  Function that is called to check whether a certain property is set
     *  @param  object          The object on which it is called
     *  @param  name            The name of the property to check
     *  @param  has_set_exists  See above
     *  @return bool
     */
    static int hasProperty(struct _zval_struct *object, struct _zval_struct *name, int has_set_exists, const struct _zend_literal *key);
    static int hasProperty(struct _zval_struct *object, struct _zval_struct *name, int has_set_exists);

    /**
     *  Function that is called when a property is removed from the project
     *  @param  object          The object on which it is called
     *  @param  member          The member to remove
     */
    static void unsetProperty(struct _zval_struct *object, struct _zval_struct *member, const struct _zend_literal *key);
    static void unsetProperty(struct _zval_struct *object, struct _zval_struct *member);

    /**
     *  Method that returns information about the function signature of a undefined method
     *  @param  object_ptr
     *  @param  method
     *  @param  method_len
     *  @param  key
     *  @return zend_function
     */
    static union _zend_function *getMethod(struct _zval_struct **object_ptr, char *method, int method_len, const struct _zend_literal *key);
    static union _zend_function *getMethod(struct _zval_struct **object_ptr, char *method, int method_len);

    /**
     *  Method that returns information about the function signature of an undefined static method
     *  @param  object_ptr
     *  @param  method
     *  @param  method_len
     *  @param  key
     *  @return zend_function
     */
    static union _zend_function *getStaticMethod(struct _zend_class_entry *entry, char* method, int method_len);

    /**
     *  Method that returns information about the __invoke() method
     *  @param  object
     *  @param  entry
     *  @param  func
     *  @param  object_ptr
     *  @return int
     */
    static int getClosure(struct _zval_struct *object, struct _zend_class_entry **entry, union _zend_function **func, struct _zval_struct **object_ptr);

    /**
     *  Function to cast the object to a different type
     *  @param  object
     *  @param  retval
     *  @param  type
     *  @return int
     */
    static int cast(struct _zval_struct *object, struct _zval_struct *retval, int type);

    /**
     *  Function to compare two objects
     *  @param  object1
     *  @param  object2
     *  @return int
     */
    static int compare(struct _zval_struct *object1, struct _zval_struct *object2);

    /**
     *  Methods that are called to serialize/unserialize an object
     *  @param  object      The object to be serialized
     *  @param  entry       The class entry to which the object belongs
     *  @param  buffer      Buffer in which to store the data
     *  @param  buf_len     Size of the bufffer
     *  @param  data        Structure describing the serialize/unserialize data
     *  @return int
     */
    static int serialize(struct _zval_struct *object, unsigned char **buffer, unsigned int *buf_len, struct _zend_serialize_data *data);
    static int unserialize(struct _zval_struct **object, struct _zend_class_entry *entry, const unsigned char *buffer, unsigned int buf_len, struct _zend_unserialize_data *data);

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
    struct _zend_class_entry *_entry = nullptr;

    /**
     *  Pointer to the entries
     *  @var    zend_function_entry[]
     */
    struct _zend_function_entry *_entries = nullptr;
    
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
     *  Base object has access to the members
     *  This is needed by the Base::store() method
     */
    friend class Base;
};
    
/**
 *  End namespace
 */
}

