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
 *  Function that is called to clean up space that is occupied by the object
 *  @param  object      The object to be deallocated
 */
static void deallocate_object(void *object TSRMLS_DC)
{
    // allocate memory for the object
    MixedObject *obj = (MixedObject *)object;
    
    // deallocate the cpp object
    if (obj->cpp) delete obj->cpp;
    
    // get rid of the object properties
    // @todo if we enable the following two lines, segmentation
    //      faults and memory corruption occurs. however, the online
    //      documentation does it like this
    //zend_hash_destroy(obj->php.properties);
    //FREE_HASHTABLE(obj->php.properties);

    // deallocate the entire object
    efree(obj);
}

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
 *  Create an object based on a certain class entry
 *  @param  entry
 *  @return MixedObject
 */
static MixedObject *allocate_object(zend_class_entry *entry)
{
    // allocate memory for the object
    MixedObject *result = (MixedObject *)emalloc(sizeof(MixedObject));
    
    // store the class entry in the newly created object
    result->php.ce = entry;

#if PHP_VERSION_ID < 50399
    // the original create_object fills the initial object with the default properties,
    // we're going to do exactly the same. start with setting up a hashtable for the props
    ALLOC_HASHTABLE(result->php.properties);

    // initialize the hash table
    zend_hash_init(result->php.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

    // initialize the properties
    zend_hash_copy(result->php.properties, &entry->default_properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else
    // version higher than 5.3 have an easier way to initialize
    object_properties_init(&result->php, entry);
#endif    
    
    // done
    return result;
}

/**
 *  Forward declaration
 */
static zend_object_value clone_object(zval *val TSRMLS_DC);

/**
 *  Store the mixed object in the PHP object cache
 *  @param  object      The object to store
 *  @param  value       The zend_object_value to initialize
 */
static void store(MixedObject *object, zend_object_value *value)
{
    // set the handlers
    value->handlers = zend_get_std_object_handlers();
    
    // we need a special handler for cloning
    value->handlers->clone_obj = clone_object;

    // the destructor and clone handlers are set to NULL. I dont know why, but they do not
    // seem to be necessary...
    value->handle = zend_objects_store_put(object, NULL, deallocate_object, NULL TSRMLS_CC);
}

/**
 *  Function that is called to create space for a cloned object
 *  @param  val                     The object to be cloned
 *  @return zend_obejct_value       The object to be created
 */
static zend_object_value clone_object(zval *val TSRMLS_DC)
{
    // @todo refactoring because there is a lot of double code here
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(val);

    // allocate memory for the new object
    MixedObject *new_object = allocate_object(entry);

    // retrieve the old object, which we are going to copy
    MixedObject *old_object = (MixedObject *)zend_object_store_get_object(val);
    
    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);
    
    // the thing we're going to return
    zend_object_value result;
    
    // store the object
    store(new_object, &result);

    // clone the members
    zend_objects_clone_members(&new_object->php, result, &old_object->php, Z_OBJ_HANDLE_P(val));
    
    // finally, construct the cpp object
    // @todo call this earlier, because it could fail
    new_object->cpp = meta->clone(old_object->cpp, result);

    // done
    return result;
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  entry                   Pointer to the class information
 *  @return zend_object_value       The newly created object
 */
static zend_object_value create_object(zend_class_entry *entry TSRMLS_DC)
{
    // allocate memory for the object
    MixedObject *new_object = allocate_object(entry);
    
    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // the thing we're going to return
    zend_object_value result;
    
    // store the object
    store(new_object, &result);

    // finally, construct the cpp object
    // @todo call this earlier because it could fail
    new_object->cpp = meta->construct(result);

    // done
    return result;
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
    entry.create_object = create_object;
    
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
    for (auto &interface : _interfaces) zend_do_implement_interface(_entry, interface);
    
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

