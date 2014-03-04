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
 *  Internal method to instantiate an object
 *  @param  name
 */
void Object::instantiate(const char *name)
{
    // convert the name into a class_entry
    auto *entry = zend_fetch_class(name, strlen(name), 0);
    if (!entry) throw Php::Exception(std::string("Unknown class name ") + name);

    // initiate the zval (which was already allocated in the base constructor)
    object_init_ex(_val, entry);
    
    // @todo    should we call methods like allocating hashtables, copying and
    //          initializing properties, et cetera????? In all example you always
    //          see such complicated and next-to-impossible-to-understand
    //          sequences of functions being called, but this object_init_ex
    //          also seems to work...
    
    // @todo    is this a memory leak? the base class first initializes a stdClass, 
    //          and then we overwrite it with a specific class
    
}

/**
 *  End namespace
 */
}

