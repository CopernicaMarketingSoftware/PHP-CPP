/**
 *  ClassBase.cpp
 *
 *  Implementation of the ClassBase class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Retrieve our C++ implementation object
 *  @param  entry
 *  @return ClassBase
 */
static ClassBase *cpp_class(zend_class_entry *entry)
{
    // we need the base class (in user space the class may have been overridden,
    // but we are not interested in these user space classes)
    while (entry->parent) entry = entry->parent;
    
#if PHP_VERSION_ID >= 50400
    // retrieve the comment (it has a pointer hidden in it to the ClassBase object)
    const char *comment = entry->info.user.doc_comment;
#else
    // retrieve the comment php5.3 style (it has a pointer hidden in it to the ClassBase object)
    const char *comment = entry->doc_comment;
#endif    
    
    // the first byte of the comment is an empty string (null character), but
    // the next bytes contain a pointer to the ClassBase class
    return *((ClassBase **)(comment + 1));
}

/**
 *  Retrieve the CPP object
 *  @param  val
 *  @return Base
 */
static Base *cpp_object(const zval *val)
{
    // retrieve the old object, which we are going to copy
    MixedObject *object = (MixedObject *)zend_object_store_get_object(val);

    // return the cpp object
    return object->cpp;
}

/**
 *  Retrieve pointer to our own object handlers
 *  @return zend_object_handlers
 */
zend_object_handlers *ClassBase::objectHandlers()
{
    // keep static structure
    static zend_object_handlers handlers;
    
    // is the object already initialized?
    static bool initialized = false;
    
    // already initialized?
    if (initialized) return &handlers;
    
    // initialize the handlers
    memcpy(&handlers, &std_object_handlers, sizeof(zend_object_handlers));
    
    // install custom clone function
    handlers.clone_obj = &ClassBase::cloneObject;
    
    // functions for the Countable interface
    handlers.count_elements = &ClassBase::countElements;
    
    // functions for the ArrayAccess interface
    handlers.write_dimension = &ClassBase::writeDimension;
    handlers.read_dimension = &ClassBase::readDimension;
    handlers.has_dimension = &ClassBase::hasDimension;
    handlers.unset_dimension = &ClassBase::unsetDimension;
    
    // functions for the magic properties handlers (__get, __set, __isset and __unset)
    handlers.write_property = &ClassBase::writeProperty;
    handlers.read_property = &ClassBase::readProperty;
    handlers.has_property = &ClassBase::hasProperty;
    handlers.unset_property = &ClassBase::unsetProperty;
    
    // remember that object is now initialized
    initialized = true;
    
    // done
    return &handlers;
}
    
/**
 *  Function that is called to create space for a cloned object
 *  @param  val                     The object to be cloned
 *  @return zend_obejct_value       The object to be created
 */
zend_object_value ClassBase::cloneObject(zval *val TSRMLS_DC)
{
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(val);

    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // retrieve the old object, which we are going to copy
    MixedObject *old_object = (MixedObject *)zend_object_store_get_object(val);

    // create a new base c++ object
    auto *cpp = meta->clone(old_object->cpp);
    
    // report error on failure
    if (!cpp) throw Php::Exception(std::string("Unable to clone ") + entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = ClassBase::objectHandlers();
    
    // store the object
    MixedObject *new_object = cpp->store(entry);

    // store the object in the object cache
    result.handle = cpp->handle();
    
    // clone the members
    zend_objects_clone_members(&new_object->php, result, &old_object->php, Z_OBJ_HANDLE_P(val));
    
    // done
    return result;
}

/**
 *  Function that is used to count the number of elements in the object
 * 
 *  If the user has implemented the Countable interface, this method will 
 *  call the count() method
 * 
 *  @param  val
 *  @param  count
 *  @return int
 */
int ClassBase::countElements(zval *object, long *count TSRMLS_DC)
{
    // does it implement the countable interface?
    Countable *countable = dynamic_cast<Countable*>(cpp_object(object));

    // if it does not implement the Countable interface, we rely on the default implementation
    if (countable) 
    {
        // the user function may throw an exception that needs to be processed
        try
        {
            // call the count function
            *count = countable->count();
            
            // done
            return SUCCESS;
        }
        catch (Exception &exception)
        {
            // process the exception
            exception.process();
            
            // unreachable
            return FAILURE;
        }
    }
    else
    {
        // Countable interface was not implemented, check if there is a default
        if (!std_object_handlers.count_elements) return FAILURE;
        
        // call default
        return std_object_handlers.count_elements(object, count);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetGet() method 
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  offset          The name of the property
 *  @param  type            The type of the variable???
 *  @return zval
 */
zval *ClassBase::readDimension(zval *object, zval *offset, int type)
{
    // what to do with the type?
    //
    // the type parameter tells us whether the dimension was read in READ
    // mode, WRITE mode, READWRITE mode or UNSET mode. 
    // 
    // In 99 out of 100 situations, it is called in regular READ mode (value 0), 
    // only when it is called from a PHP script that has statements like 
    // $x =& $object["x"], $object["x"]["y"] = "something" or unset($object["x"]["y"]), 
    // the type parameter is set to a different value.
    //
    // But we must ask ourselves the question what we should be doing with such
    // cases. Internally, the object most likely has a full native implementation,
    // and the property that is returned is just a string or integer or some
    // other value, that is temporary WRAPPED into a zval to make it accessible
    // from PHP. If someone wants to get a reference to such an internal variable,
    // that is in most cases simply impossible.
    
     
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // the C++ code may throw an exception
        try
        {
            // ArrayAccess is implemented, call function
            Value value = arrayaccess->offsetGet(offset);
            
            // because we do not want the value object to destruct the zval when
            // it falls out of scope, we detach the zval from it, if this is a regular
            // read operation we can do this right away
            if (type == 0) return value.detach();
            
            // this is a more complicated read operation, the scripts wants to get
            // deeper access to the returned value. This, however, is only possible
            // if the value has more than once reference (if it has a refcount of one,
            // the value object that we have here is the only instance of the zval,
            // and it is simply impossible to return a reference or so
            if (value.refcount() <= 1) return value.detach(); 
            
            // we're dealing with an editable zval, return a reference variable
            return Value(value.detach(), true).detach();
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
            
            // unreachable
            return Value(nullptr).detach();
        }
    }
    else
    {
        // ArrayAccess not implemented, check if there is a default handler
        if (!std_object_handlers.read_dimension) return nullptr;
        
        // call default
        return std_object_handlers.read_dimension(object, offset, type);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetSet() method 
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  offset          The name of the property
 *  @param  value           The new value
 *  @return zval
 */
void ClassBase::writeDimension(zval *object, zval *offset, zval *value)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // method may throw an exception
        try
        {
            // set the value
            arrayaccess->offsetSet(offset, value);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space
            exception.process();
        }
    }
    else
    {
        // ArrayAccess not interface, check if there is a default handler
        if (!std_object_handlers.write_dimension) return;
        
        // call the default
        std_object_handlers.write_dimension(object, offset, value);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetExists() method
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to check
 *  @param  check_empty     Was this an isset() call, or an empty() call?
 *  @return bool
 */
int ClassBase::hasDimension(zval *object, zval *member, int check_empty)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // user implemented callbacks could throw an exception
        try
        {
            // check if the member exists
            if (!arrayaccess->offsetExists(member)) return false;
            
            // we know for certain that the offset exists, but should we check
            // more, like whether the value is empty or not?
            if (!check_empty) return true;
            
            // it should not be empty
            return !arrayaccess->offsetGet(member).isEmpty();
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
            
            // unreachable
            return false;
        }
    }
    else
    {
        // ArrayAccess interface is not implemented, check if there is a default handler
        if (!std_object_handlers.has_dimension) return 0;

        // call default
        return std_object_handlers.has_dimension(object, member, check_empty);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetUnset() method
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to remove
 */
void ClassBase::unsetDimension(zval *object, zval *member)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // user implemented code could throw an exception
        try
        {
            // remove the member
            arrayaccess->offsetUnset(member);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
        }
    }
    else
    {
        // ArrayAccess is not implemented, is a default handler available?
        if (!std_object_handlers.unset_dimension) return;
        
        // call the default
        std_object_handlers.unset_dimension(object, member);
    }
}

/**
 *  Function that is called when a property is read
 *  @param  object
 *  @param  name
 *  @param  type
 *  @param  key
 *  @return val
 */
#if PHP_VERSION_ID < 50399
zval *ClassBase::readProperty(zval *object, zval *name, int type)
#else
zval *ClassBase::readProperty(zval *object, zval *name, int type, const struct _zend_literal *key)
#endif
{
    // what to do with the type?
    //
    // the type parameter tells us whether the property was read in READ
    // mode, WRITE mode, READWRITE mode or UNSET mode. 
    // 
    // In 99 out of 100 situations, it is called in regular READ mode (value 0), 
    // only when it is called from a PHP script that has statements like 
    // $x =& $object->x, $object->x->y = "something" or unset($object->x->y)
    // the type parameter is set to a different value.
    //
    // But we must ask ourselves the question what we should be doing with such
    // cases. Internally, the object most likely has a full native implementation,
    // and the property that is returned is just a string or integer or some
    // other value, that is temporary WRAPPED into a zval to make it accessible
    // from PHP. If someone wants to get a reference to such an internal variable,
    // that is in most cases simply impossible.

    // the default implementation throws an exception, so by catching 
    // the exception we know if the object was implemented by the user or not
    try
    {
        // retrieve value
        Value value = cpp_object(object)->__get(name);

        // because we do not want the value object to destruct the zval when
        // it falls out of scope, we detach the zval from it, if this is a regular
        // read operation we can do this right away
        if (type == 0) return value.detach();
        
        // this is a more complicated read operation, the scripts wants to get
        // deeper access to the returned value. This, however, is only possible
        // if the value has more than once reference (if it has a refcount of one,
        // the value object that we have here is the only instance of the zval,
        // and it is simply impossible to return a reference or so
        if (value.refcount() <= 1) return value.detach(); 
        
        // we're dealing with an editable zval, return a reference variable
        return Value(value.detach(), true).detach();
    }
    catch (const NotImplemented &exception)
    {
        // __get() function was not overridden by the user
        if (!std_object_handlers.read_property) return nullptr;
        
        // call default
#if PHP_VERSION_ID < 50399
        return std_object_handlers.read_property(object, name, type);
#else
        return std_object_handlers.read_property(object, name, type, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return Value(nullptr).detach();
    }
}

/**
 *  Function that is called when a property is set / updated
 * 
 *  This is the handler for the __set() function, and is called when a property
 *  is updated.
 * 
 *  @param  object          The object on which it is called
 *  @param  name            The name of the property
 *  @param  value           The new value
 *  @param  key             ???
 *  @return zval
 */
#if PHP_VERSION_ID < 50399
void ClassBase::writeProperty(zval *object, zval *name, zval *value)
#else
void ClassBase::writeProperty(zval *object, zval *name, zval *value, const struct _zend_literal *key)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __set method
    try
    {
        // call the default
        cpp_object(object)->__set(name, value);
    }
    catch (const NotImplemented &exception)
    {
        // __set() function was not overridden by user, check if there is a default
        if (!std_object_handlers.write_property) return;
        
        // call the default
#if PHP_VERSION_ID < 50399
        std_object_handlers.write_property(object, name, value);
#else
        std_object_handlers.write_property(object, name, value, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
    }
}

/**
 *  Function that is called to check whether a certain property is set
 *  for an object
 * 
 *  This is the handler for the __isset() function, and is called when a PHP
 *  script checks if a certain property is set.
 * 
 *  The has_set_exists parameter can have the following values:
 *
 *      0 (has) whether property exists and is not NULL
 *      1 (set) whether property exists and is true
 *      2 (exists) whether property exists
 * 
 *  @param  object          The object on which it is called
 *  @param  name            The name of the property to check
 *  @param  has_set_exists  See above
 *  @param  key             ???
 *  @return bool
 */
#if PHP_VERSION_ID < 50399
int ClassBase::hasProperty(zval *object, zval *name, int has_set_exists)
#else
int ClassBase::hasProperty(zval *object, zval *name, int has_set_exists, const struct _zend_literal *key)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __isset method
    try
    {
        // get the cpp object
        Base *base = cpp_object(object);
        
        // call the C++ object
        if (!base->__isset(name)) return false;
        
        // property exists, but what does the user want to know
        if (has_set_exists == 2) return true;
        
        // we have to retrieve the property
        Value value = base->__get(name);
        
        // should we check on NULL?
        switch (has_set_exists) {
        case 0:     return value.type() != Type::Null;
        default:    return value.boolValue();
        }
    }
    catch (const NotImplemented &exception)
    {
        // __isset was not implemented, do we have a default?
        if (!std_object_handlers.has_property) return 0;

        // call default
#if PHP_VERSION_ID < 50399
        return std_object_handlers.has_property(object, name, has_set_exists);
#else
        return std_object_handlers.has_property(object, name, has_set_exists, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return false;
    }
}

/**
 *  Function that is called when a property is removed from the project
 * 
 *  This is the handler for the __unset() method
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to remove
 *  @param  key
 */
#if PHP_VERSION_ID < 50399
void ClassBase::unsetProperty(zval *object, zval *member)
#else
void ClassBase::unsetProperty(zval *object, zval *member, const struct _zend_literal *key)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __unset method
    try
    {
        // call the __unset method
        cpp_object(object)->__unset(member);
    }
    catch (const NotImplemented &exception)
    {
        // __unset was not implemented, do we have a default?
        if (!std_object_handlers.unset_property) return;
        
        // call the default
#if PHP_VERSION_ID < 50399
        std_object_handlers.unset_property(object, member);
#else
        std_object_handlers.unset_property(object, member, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
    }
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  entry                   Pointer to the class information
 *  @return zend_object_value       The newly created object
 */
zend_object_value ClassBase::createObject(zend_class_entry *entry TSRMLS_DC)
{
    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // create a new base C++ object
    auto *cpp = meta->construct();

    // report error on failure
    if (!cpp) throw Php::Exception(std::string("Unable to instantiate ") + entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = ClassBase::objectHandlers();
    
    // store the object
    cpp->store(entry);

    // store the object in the object cache
    result.handle = cpp->handle();
    
    // done
    return result;
}

/**
 *  Function to create a new iterator to iterate over an object
 *  @param  entry                   The class entry
 *  @param  object                  The object to iterate over
 *  @param  by_ref                  ?????
 *  @return zend_object_iterator*   Pointer to the iterator
 */
zend_object_iterator *ClassBase::getIterator(zend_class_entry *entry, zval *object, int by_ref)
{
    // by-ref is not possible (copied from SPL)
    if (by_ref) throw Php::Exception("Foreach by ref is not possible");
    
    // retrieve the traversable object
    Traversable *traversable = dynamic_cast<Traversable*>(cpp_object(object));
    
    // user may throw an exception in the getIterator() function
    try
    {
        // create an iterator
        auto *iterator = traversable->getIterator();
        
        // return the implementation
        return iterator->implementation();
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return nullptr;
    }
}

/**
 *  Destructor
 */
ClassBase::~ClassBase()
{
    // destruct the entries
    if (_entries) delete[] _entries;

    // php 5.3 deallocates the doc_comment by iself
#if PHP_VERSION_ID >= 50400    
    if (_comment) free(_comment);
#endif
}

/**
 *  Retrieve an array of zend_function_entry objects that hold the 
 *  properties for each method. This method is called at extension
 *  startup time to register all methods.
 * 
 *  @param  classname       The class name
 *  @return zend_function_entry[]
 */
const struct _zend_function_entry *ClassBase::entries()
{
    // already initialized?
    if (_entries) return _entries;
    
    // allocate memory for the functions
    _entries = new zend_function_entry[_methods.size() + 1];
    
    // keep iterator counter
    int i = 0;

    // loop through the functions
    for (auto &method : _methods)
    {
        // retrieve entry
        zend_function_entry *entry = &_entries[i++];

        // let the function fill the entry
        method->initialize(entry, _name);
    }

    // last entry should be set to all zeros
    zend_function_entry *last = &_entries[i];

    // all should be set to zero
    memset(last, 0, sizeof(zend_function_entry));

    // done
    return _entries;
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
 *  @param  prefix      namespace prefix
 */
void ClassBase::initialize(const std::string &prefix)
{
    // the class entry
    zend_class_entry entry;

    // update the name
    if (prefix.size() > 0) _name = prefix + "\\" + _name;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), entries());

    // we need a special constructor
    entry.create_object = &ClassBase::createObject;
    
    // and a special function for retrieving the iterator (but only if this is
    // a traversable class)
    if (traversable()) entry.get_iterator = &ClassBase::getIterator;
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);
    
    // allocate doc comment to contain an empty string + a hidden pointer
    if (!_comment)
    {
        // allocate now
        _comment = (char *)malloc(1 + sizeof(ClassBase *));
        
        // empty string on first position
        _comment[0] = '\0';
        
        // this pointer has to be copied to temporary pointer, as &this causes compiler error
        ClassBase *base = this;
        
        // copy the 'this' pointer to the doc-comment
        memcpy(_comment+1, &base, sizeof(ClassBase *));
    }
    
    // store pointer to the class in the unused doc_comment member
#if PHP_VERSION_ID >= 50400    
    _entry->info.user.doc_comment = _comment;
#else
    // and store the wrapper inside the comment
    _entry->doc_comment = _comment;
#endif

    // set access types flags for class
    _entry->ce_flags = (int)_type;
    
    // mark the interfaces as being implemented
    for (auto &interface : _interfaces) 
    {
        // implement interface
        zend_do_implement_interface(_entry, *interface);
    }
    
    // declare all member variables
    for (auto &member : _members) member->initialize(_entry);
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_0 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_1 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_2 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_3 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_4 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_5 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_6 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, method_callback_7 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add an abstract method to the class
 *  @param  name        Name of the method
 *  @param  flags       Optional flags (like public or protected)
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, Abstract | flags, args));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, std::nullptr_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<NullMember>(name, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int16_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int32_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int64_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, bool value, int flags)
{
    // add property
    _members.push_back(std::make_shared<BoolMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, char value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, &value, 1, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, const std::string &value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, const char *value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, value, strlen(value), flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, double value, int flags)
{
    // add property
    _members.push_back(std::make_shared<FloatMember>(name, value, flags));
}

/**
 *  End namespace
 */
}

