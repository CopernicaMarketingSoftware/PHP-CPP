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
 *  Function that is called to create space for a cloned object
 *  @param  object      The object to be cloned
 *  @param  clone       The address that should become the clone
 */
static void clone_object(void *object, void **clone TSRMLS_DC)
{
    // @todo implementation
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  type        Pointer to the class
 */
static zend_object_value create_object(zend_class_entry *type TSRMLS_DC)
{
    // allocate memory for the object
    MixedObject *object = (MixedObject *)emalloc(sizeof(MixedObject));
    
    // find base object
    zend_class_entry *base = type;
    while (base->parent) base = base->parent;
    
    // retrieve the classinfo object
#if PHP_VERSION_ID >= 50400
    ClassBase *info = (ClassBase *)base->info.user.doc_comment;
#else
    ClassBase *info = *((ClassBase **)base->doc_comment);
#endif    
    
    // store the class
    object->php.ce = type;

    // the original create_object fills the initial object with the default properties,
    // we're going to do exactly the same. start with setting up a hashtable for the props
    ALLOC_HASHTABLE(object->php.properties);

    // initialize the hash table
    zend_hash_init(object->php.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    
    // initialize the properties
#if PHP_VERSION_ID < 50399
    zend_hash_copy(object->php.properties, &(type->default_properties),
                   (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else
    object_properties_init(&(object->php), type);
#endif    

    // the thing we're going to return
    zend_object_value result;

    // use default object handlers
    result.handlers = zend_get_std_object_handlers();
    
    // put the object in the storage, and assign a method for deallocating and cloning
    result.handle = zend_objects_store_put(object, NULL, deallocate_object, clone_object TSRMLS_CC); 

    // finally, construct the cpp object
    object->cpp = info->construct();

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
 */
void ClassBase::initialize()
{
    // the class entry
    zend_class_entry entry;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), entries());

    // we need a special constructor
    entry.create_object = create_object;
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);

    // store pointer to the class in the unused doc_comment member
#if PHP_VERSION_ID >= 50400    
    _entry->info.user.doc_comment = (const char *)this;
#else
    /**
     *  PHP 5.3 will free the doc_comment pointer if it
     *  is not NULL, which will result in the classinfo
     *  object being freed without being destructed
     *  properly, leading to segfaults when the destruct
     *  is called at a later stage (during module_shutdown).
     *
     *  To prevent this we create an extra pointer that
     *  points to our this pointer. We do *not* free this
     *  pointer ourselves, because PHP does this. This
     *  way it does not free the classinfo.
     */
    char **wrapper = (char**)malloc(sizeof(char**));

    // have the wrapper point to us
    *wrapper = (char *)this;

    // and store the wrapper inside the comment
    _entry->doc_comment = (char *)wrapper;
#endif

    // set access types flags for class
    _entry->ce_flags = _flags;
    
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
void ClassBase::add(const char *name, method_callback_0 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.insert(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::add(const char *name, method_callback_1 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.insert(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::add(const char *name, method_callback_2 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.insert(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::add(const char *name, method_callback_3 callback, int flags, const Arguments &args)
{
    // add the method
    _methods.insert(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, std::nullptr_t value, int flags)
{
    // add property
    _members.insert(std::make_shared<NullMember>(name, flags));
}
    
/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, int16_t value, int flags)
{
    // add property
    _members.insert(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, int32_t value, int flags)
{
    // add property
    _members.insert(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, int64_t value, int flags)
{
    // add property
    _members.insert(std::make_shared<LongMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, bool value, int flags)
{
    // add property
    _members.insert(std::make_shared<BoolMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, char value, int flags)
{
    // add property
    _members.insert(std::make_shared<StringMember>(name, &value, 1, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, const std::string &value, int flags)
{
    // add property
    _members.insert(std::make_shared<StringMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, const char *value, int flags)
{
    // add property
    _members.insert(std::make_shared<StringMember>(name, value, strlen(value), flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::add(const char *name, double value, int flags)
{
    // add property
    _members.insert(std::make_shared<FloatMember>(name, value, flags));
}
    
/**
 *  End namespace
 */
}

