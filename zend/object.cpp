/**
 *  Object.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 - 2019 Copernica BV
 */
#include "includes.h"
#include "string.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor to create a new instance of a builtin class
 *
 *  @param  name        Name of the class to instantiate
 *  @param  base        The C++ object to wrap
 */
Object::Object(const char *name, Base *base) : Value()
{
    // does the object already have a handle?
    if (base->implementation())
    {
        // the object is already instantiated, we can assign it to this object
        operator=(Value(base));
    }
    else
    {
        // this is a brand new object that should be allocated, the C++ instance
        // is already there (created by the extension) but it is not yet stored
        // in PHP, find out the classname first
        auto *entry = zend_fetch_class(String{ name }, ZEND_FETCH_CLASS_SILENT);
        if (!entry) throw Error(std::string("Unknown class name ") + name);

        // construct an implementation (this will also set the implementation
        // member in the base object), this is a self-destructing object that
        // will be destructed when the last reference to it has been removed,
        // we already set the reference to zero
        new ObjectImpl(entry, base, ClassImpl::objectHandlers(entry), 0);

        // now we can store it
        operator=(Value(base));

        // install the object handlers
        Z_OBJ_P(_val)->handlers = ClassImpl::objectHandlers(entry);
    }
}

/**
 *  Constructor in case the class entry is already known
 *
 *  @param  entry       Class entry
 *  @param  base        The C++ object to wrap
 */
Object::Object(zend_class_entry *entry, Base *base) : Value()
{
    // does the object already have a handle?
    if (base->implementation())
    {
        // the object is already instantiated, we can assign it to this object
        operator=(Value(base));
    }
    else
    {
        // construct an implementation (this will also set the implementation
        // member in the base object), this is a self-destructing object that
        // will be destructed when the last reference to it has been removed,
        // we already set the reference to zero
        new ObjectImpl(entry, base, ClassImpl::objectHandlers(entry), 0);

        // now we can store it
        operator=(Value(base));
    }
}

/**
 *  Copy constructor is valid if the passed in object is also an object,
 *  or when it is a string holding a classname
 *  @param  that        An other object
 */
Object::Object(const Value &value) : Value()
{
    // when a string is passed in, we are going to make a new instance of the
    // passed in string
    if (value.isString())
    {
        // instantiate the object
        if (instantiate(value)) call("__construct");
    }
    else
    {
        // this simply copies the other object
        operator=(value);
    }
}

/**
 *  Internal method to instantiate an object
 *  @param  name        Name of the class to instantiate
 *  @return bool        True if there is a __construct function
 */
bool Object::instantiate(const char *name)
{
    // convert the name into a class_entry
    auto *entry = zend_fetch_class(String{ name }, ZEND_FETCH_CLASS_SILENT);
    if (!entry) throw Error(std::string("Unknown class name ") + name);

    // initiate the zval (which was already allocated in the base constructor)
    object_init_ex(_val, entry);

    // @todo    should we call methods like allocating hashtables, copying and
    //          initializing properties, et cetera????? In all example you always
    //          see such complicated and next-to-impossible-to-understand
    //          sequences of functions being called, but this object_init_ex
    //          also seems to work...

    // @todo    is this a memory leak? the base class first initializes a stdClass,
    //          and then we overwrite it with a specific class

    // initialize the string for __construct only once
    static String construct{ "__construct" };

    // return whether there is a __construct function
    return zend_hash_exists(&entry->function_table, construct);
}

/**
 *  End namespace
 */
}

