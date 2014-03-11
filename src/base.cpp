/**
 *  Base.cpp
 *
 *  Implementation file for the base of all classes
 *
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
 *  Store the object in the PHP object cache
 *  @param  entry               Class entry
 *  @return MixedObject
 */
MixedObject *Base::store(zend_class_entry *entry)
{
    // allocate memory for the object
    MixedObject *result = (MixedObject *)emalloc(sizeof(MixedObject));
    
    // store the new c++ object
    result->cpp = this;
    
    // store the class entry in the newly created object
    result->php.ce = entry;
    
    // @todo is this really necessary - and when do we destruct this data?
    // (if we remove this code, everything breaks down in a for ($object as $k => $v) loop)

    // the original create_object fills the initial object with the default properties,
    // we're going to do exactly the same. start with setting up a hashtable for the props
    ALLOC_HASHTABLE(result->php.properties);

    // initialize the hash table
    zend_hash_init(result->php.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50399
    // initialize the properties
    zend_hash_copy(result->php.properties, &entry->default_properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else
    // version higher than 5.3 have an easier way to initialize
    object_properties_init(&result->php, entry);
#endif    

    // the destructor and clone handlers are set to NULL. I dont know why, but they do not
    // seem to be necessary...
    _handle = zend_objects_store_put(result, NULL, deallocate_object, NULL TSRMLS_CC);
    
    // done
    return result;
}

/**
 *  Overridable method that is called to check if a property is set
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @return bool
 */
bool Base::__isset(const Php::Value &key)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the unset function can be called
    throw NotImplemented();
}

/**
 *  Overridable method that is called to set a new property
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @param  value
 */
void Base::__set(const Php::Value &key, const Php::Value &value)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the unset function can be called
    throw NotImplemented();
}

/**
 *  Retrieve a property
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param  key
 *  @return value
 */
Php::Value Base::__get(const Php::Value &key)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}

/**
 *  Remove a member
 * 
 *  The default implementation does nothing, and the script will fall back
 *  to accessing the regular object properties
 * 
 *  @param key
 */
void Base::__unset(const Php::Value &key)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
}

/**
 *  Call a method
 * 
 *  This method is called when a method is called from the PHP script that
 *  was not explicitly defined. You can use this to catch variable method
 *  names, or to support all thinkable method names.
 * 
 *  @param  method      Name of the method that was called
 *  @param  params      The parameters that were passed to the function
 *  @return Value       The return value
 */
Value Base::__call(const char *method, Parameters &params)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}

/**
 *  Call the class as if it was a function
 * 
 *  This method is called when a an object is used with () operators:
 *  $object(). You can override this method to make objects callable.
 * 
 *  @param  params      The parameters that were passed to the function
 *  @return Value       The return value
 */
Value Base::__invoke(Parameters &params)
{
    // throw an exception that will be caught in the ClassBase class, 
    // so that the default implementation of the function can be called
    throw NotImplemented();
    
    // unreachable code
    return nullptr;
}
    
/**
 *  End namespace
 */
}

