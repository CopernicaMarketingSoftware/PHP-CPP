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
union _zend_function;

/**
 *  Set up namespace
 */
namespace Php {

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
 *  Forward declarations
 */
class Method;
class Member;

/**
 *  Class definition
 */
class ClassBase
{
protected:
    /**
     *  Protected constructor
     *  @param  classname   Class name
     *  @param  type        The class type
     */
    ClassBase(const char *classname, ClassType type = ClassType::Regular) : _name(classname), _type(type) {}
    
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
        _interfaces(that._interfaces),
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
        _interfaces(std::move(that._interfaces)),
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
     *  Construct a new instance of the object
     *  @return Base
     */
    virtual Base* construct() const = 0;
    
    /**
     *  Create a clone of an object
     *  @param  orig
     *  @return Base
     */
    virtual Base *clone(Base *orig) const = 0;

    /**
     *  Is this a traversable class?
     *  @return bool
     */
    virtual bool traversable() const = 0;

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
    void method(const char *name, method_callback_0, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_1, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_2, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_3, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_4, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_5, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_6, int flags=0, const Arguments &args = {});
    void method(const char *name, method_callback_7, int flags=0, const Arguments &args = {});

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
     *  Add an implemented interface
     *  
     *  This can only be used to register interfaces that are already defined
     *  by Zend, and not for user space interface or custom extension interfaces.
     *  This is probably not so much of a problem, as this feature is mostly
     *  useful for interfaces like 'Countable', 'ArrayAccess', 'Iterator', et
     *  cetera. Interfaces defined in user space are in normal operations
     *  inaccessible (user space code normally runs after the extension has been 
     *  set up) - so we do not need a feature to set these.
     * 
     *  It does however make sense to support implementing extension-specific
     *  interface. We may add this feature in the future.
     * 
     *  This method is called _during_ the get_module() call when all classes
     *  are defined by the extension. However, at that time the Zend engine has
     *  not yet initialized the zend_class_entry's with the interface addresses.
     *  That's why we ask for a pointer-to-a-pointer. Later, when the classes
     *  are really registered, the Zend engine is with registering interfaces
     *  and the pointers point to a valid variable.
     * 
     *  @param  interface
     */
    void interface(struct ::_zend_class_entry **interface)
    {
        // register the interface
        _interfaces.push_back(interface);
    }

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
     *  Static member functions to clone objects based on this class
     *  @param  val                     The object to be cloned
     *  @return zend_object_value       Object info
     */
    static struct _zend_object_value cloneObject(struct _zval_struct *val);

    /**
     *  Function that is called when an instance of the class needs to be created.
     *  This function will create the C++ class, and the PHP object
     *  @param  entry                   Pointer to the class information
     *  @return zend_object_value       The newly created object
     */
    static struct _zend_object_value createObject(struct _zend_class_entry *entry);

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
    static struct _zend_object_handlers *objectHandlers();

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
     *  Method that is called when a undefined method is invoked
     *  @param  method
     *  @param  ht
     *  @param  return_value
     *  @param  return_value_ptr
     *  @param  this_ptr
     *  @param  return_value_used
     *  @param  tsrm_ls
     *  @return integer
     */
    static int callMethod(const char *method, int ht, struct _zval_struct *return_value, struct _zval_struct **return_value_ptr, struct _zval_struct *this_ptr, int return_value_used);
    static int callMethod(char *method, int ht, struct _zval_struct *return_value, struct _zval_struct **return_value_ptr, struct _zval_struct *this_ptr, int return_value_used);

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
     *  Method that returns information about the __invoke() method
     *  @param  object
     *  @param  entry
     *  @param  func
     *  @param  object_ptr
     *  @return int
     */
    static int getClosure(struct _zval_struct *object, struct _zend_class_entry **entry, union _zend_function **func, struct _zval_struct **object_ptr);

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
     *  All interfaces that are implemented
     *  @var    std::list
     */
    std::list<struct ::_zend_class_entry**> _interfaces;
    
};
    
/**
 *  End namespace
 */
}

