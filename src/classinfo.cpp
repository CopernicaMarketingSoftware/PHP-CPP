/**
 *  ClassInfo.cpp
 *
 *  Implementation for the class info
 *
 *  @documentation private
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Structure that combines a C++ object with a zend object
 */
struct MixedObject
{
    zend_object php;
    Base *cpp;
};

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
	//		faults and memory corruption occurs. however, the online
	//		documentation does it like this
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
    std::cout << "clone_object" << std::endl;
    
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
    
    // retrieve the classinfo object
    _ClassInfo *info = (_ClassInfo *)type->info.user.doc_comment;
    
    // construct the cpp object
    object->cpp = info->construct();
    
    // store the class
    object->php.ce = type;

    // the original create_object fills the initial object with the default properties,
    // we're going to do exactly the same. start with setting up a hashtable for the props
    ALLOC_HASHTABLE(object->php.properties);

    // initialize the hash table
    zend_hash_init(object->php.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    
    // initialize the properties
    object_properties_init(&(object->php), type);

    // the thing we're going to return
    zend_object_value result;

    // use default object handlers
    result.handlers = zend_get_std_object_handlers();
    
    // put the object in the storage, and assign a method for deallocating and cloning
    result.handle = zend_objects_store_put(object, NULL, deallocate_object, clone_object TSRMLS_CC); 

    // done
    return result;
}

/**
 *  Function that is called by the Zend engine every time the constructor gets called
 *  @param  ht      
 *  @param  return_value
 *  @param  return_value_ptr
 *  @param  this_ptr
 *  @param  return_value_used
 *  @param  tsrm_ls
 */
static void invoke_constructor(INTERNAL_FUNCTION_PARAMETERS)
{
    // get the mixed object
    MixedObject *obj = (MixedObject *)zend_object_store_get_object(this_ptr TSRMLS_CC);

    // construct parameters
    Parameters params(ZEND_NUM_ARGS());

    // call the constructor
    obj->cpp->__construct(*PHPCPP_G(environment), params);
}

/**
 *  Function that is called by the Zend engine every time the destructor gets called
 *  @param  ht      
 *  @param  return_value
 *  @param  return_value_ptr
 *  @param  this_ptr
 *  @param  return_value_used
 *  @param  tsrm_ls
 */
static void invoke_destructor(INTERNAL_FUNCTION_PARAMETERS)
{
    // get the mixed object
    MixedObject *obj = (MixedObject *)zend_object_store_get_object(this_ptr TSRMLS_CC);

    // call the destructor
    obj->cpp->__destruct(*PHPCPP_G(environment));
}

/**
 *  Constructor
 *  @param  name
 */
_ClassInfo::_ClassInfo(const char *name) : _name(name), _entry(NULL) 
{
    // allocate internal functions
    _constructor = new InternalFunction(invoke_constructor, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR);
    _destructor = new InternalFunction(invoke_destructor, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR);
}

/**
 *  Destructor
 */
_ClassInfo::~_ClassInfo() 
{
    // deallocate internal function
    delete _constructor;
    delete _destructor;
}

/**
 *  Initialize the class
 *  @param  mixed       Optional threading ID 
 */
void _ClassInfo::initialize(TSRMLS_D)
{
    // the class entry
    zend_class_entry entry;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), NULL);

    // we need a special constructor
    entry.create_object = create_object;
    entry.constructor = _constructor->function();
    entry.destructor = _destructor->function();
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);

    // store pointer to the class in the unused doc_comment member
    _entry->info.user.doc_comment = (const char *)this;
    
    // initialize the entry
    initialize(_entry);
}

/**
 *  End of namespace
 */
}
 
