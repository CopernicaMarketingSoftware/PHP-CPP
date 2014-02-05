/**
 *  ClassInfo.cpp
 *
 *  Implementation for the class info
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
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
    _ClassInfo *info = (_ClassInfo *)base->info.user.doc_comment;
#else
    _ClassInfo *info = (_ClassInfo *)base->doc_comment;
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
 *  Constructor
 *  @param  name
 */
_ClassInfo::_ClassInfo(const char *name) : _name(name), _entry(NULL) 
{
}

/**
 *  Destructor
 */
_ClassInfo::~_ClassInfo() 
{
}

/**
 *  Initialize the class
 *  @param  mixed       Optional threading ID 
 */
void _ClassInfo::initialize(TSRMLS_DC)
{
    // the class entry
    zend_class_entry entry;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), methods());

    // we need a special constructor
    entry.create_object = create_object;
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);

    // store pointer to the class in the unused doc_comment member
#if PHP_VERSION_ID >= 50400    
    _entry->info.user.doc_comment = (const char *)this;
#else
    _entry->doc_comment = (char *)this;    
#endif

    std::cerr << "\x1b[31;47m _ClassInfo::initialize  \x1b[0m" << std::endl;

#ifdef ZTS
    std::cerr << "\x1b[34;47m ZTS defined  \x1b[0m" << std::endl;
#else /* non ZTS */
    std::cerr << "\x1b[34;47m ZTS NOT defined  \x1b[0m" << std::endl;
#endif /* ZTS */

    // initialize the entry
    initialize(_entry);
}

/**
 *  set access types flags for class
 */
void _ClassInfo::seFlags(struct _zend_class_entry *entry, int flags) {
    entry->ce_flags |= flags;
}


/*
template <typename T>
void ClassInfo<T>::initialize(struct _zend_class_entry *entry) {
        

    //_entry->ce_flags |= ZEND_ACC_INTERFACE;
    entry->ce_flags |= getFlags();
    
            //= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS,     //0x20
            //= ZEND_ACC_FINAL_CLASS,                 //0x40
            //= ZEND_ACC_INTERFACE,                   //0x80
            //= ZEND_ACC_TRAIT,                       //0x120

    std::cerr << "\x1b[37;43m _entry->ce_flags =  \x1b[0m" << entry->ce_flags << std::endl;


    
    // pass to the entry
    _type.initialize(entry);
}
*/

/**
 *  End of namespace
 */
}
 
