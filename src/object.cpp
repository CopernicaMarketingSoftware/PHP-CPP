/**
 *  Object.cpp
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
 *  Constructor
 *  @param  name
 */
Object::Object(const char *name)
{
    // step 1: convert the name into a class_entry
    auto *entry = zend_fetch_class(name, strlen(name), 0);
    if (!entry) throw Php::Exception("Unknown class name");

    // initiate the zval (which was already allocated in the base constructor)
    object_init_ex(_val, entry);
    
//    // is there a special function to create the object?
//    if (entry->create_object)
//    {
//        // create the object
//        zend_object_value value = entry->create_object(entry);
//        
//        // wrap this in the zval (which was already allocated in the base constructor)
//        Z_TYPE_P(_val) = IS_OBJECT;
//        Z_OBJVAL_P(_val) = value;
//    }
//    else
//    {
//    }
    
    // @todo    should we call methods like allocating hashtables, copyint and
    //          initializing properties, et cetera?????
    
    // call the constructor
    call("__construct");
}

/**
 *  End namespace
 */
}

