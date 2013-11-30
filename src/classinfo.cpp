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
    
    // find base object
    zend_class_entry *base = type;
    while (base->parent) base = base->parent;
    
    // retrieve the classinfo object
    _ClassInfo *info = (_ClassInfo *)base->info.user.doc_comment;
    
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

    // finally, construct the cpp object
    object->cpp = info->construct();

    //std::cout << "Allocate object" << std::endl;
    //std::cout << object->cpp << " " << object << std::endl;

    // done
    return result;
}

/**
 *  Constructor
 *  @param  name
 */
_ClassInfo::_ClassInfo(const char *name) : _name(name), _entry(NULL), _base_type(NULL)
{
}

/**
 *  Constructor
 *  @param  name
 */
_ClassInfo::_ClassInfo(const char *name, _ClassInfo * base_class) : _name(name), _entry(NULL), _base_type(base_class)
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
	//@todo clean up this method by separating registration of classes with a base class and exception classes

    // the class entry
    zend_class_entry entry;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), methods());

    // we need a special constructor
    entry.create_object = create_object;
    
    // do we need to set a base class?
    if(_name == Php::Constants::ExceptionClassName)
	{
		// We are dealing with the exception base class being registered, set the
		// base class to the zend exception default
		_entry = zend_register_internal_class_ex(&entry, zend_exception_get_default(), (char*)Php::Constants::PhpExceptionBaseClass.c_str() TSRMLS_CC);
	}
    else if(_base_type == NULL || _base_type->_entry == NULL)
    {
    	// we do not have a base class, just register the class
    	_entry = zend_register_internal_class(&entry TSRMLS_CC);
    }
    else if(_base_type->_entry != NULL)
    {
    	// copy the name of the base class into a new string literal
    	std::string class_name = std::string(_base_type->_entry->name);
    	char *class_name_writeable = new char[class_name.size() + 1];
    	std::copy(class_name.begin(), class_name.end(), class_name_writeable);
    	class_name_writeable[class_name.size()] = '\0';

    	// if the base class is 'PhpCppException', make the default Zend exception
    	// class the base class, this is because all exceptions in PHP must inherit
    	// from the default PHP Exception class, and our Exception class (named PhpCppException)
    	// represents the PHP Exception class..
    	if(class_name == Php::Constants::ExceptionClassName.c_str())
    	{
    		_entry = zend_register_internal_class_ex(&entry, zend_exception_get_default(), (char*)Php::Constants::PhpExceptionBaseClass.c_str() TSRMLS_CC);
    	}
    	else
    	{
    		_entry = zend_register_internal_class_ex(&entry, _base_type->_entry, class_name_writeable TSRMLS_CC);
    	}
    }

    // store pointer to the class in the unused doc_comment member
    _entry->info.user.doc_comment = (const char *)this;
    
    // initialize the entry
    initialize(_entry);
}

/**
 *  End of namespace
 */
}
 
