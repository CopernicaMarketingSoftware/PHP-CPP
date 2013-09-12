/**
 *  Environment.cpp
 *
 *  Implementation of the environment class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace Php {

/**
 *  Get access to a global variable
 *  @param  name
 *  @return Global
 */
Global Environment::operator[](const char *name)
{
    // pointer to a zval
    zval **varvalue;
    
    // check if the variable already exists
    if (zend_hash_find(&EG(symbol_table), name, strlen(name)+1, (void**)&varvalue) == FAILURE) 
    {
        // the variable does not already exist, return a global object
        // that will automatically set the value when it is updated
        return Global(name);
    }
    else
    {
        // we are in the happy situation that the variable exists, we turn
        // this value into a reference value, and return that
        return Global(name, *varvalue);
    }
}

/**
 *  Get access to a global variable
 *  @param  name
 *  @return Global
 */
Global Environment::operator[](const std::string &name)
{
    // pointer to a zval
    zval **varvalue;
    
    // check if the variable already exists
    if (zend_hash_find(&EG(symbol_table), name.c_str(), name.size()+1, (void**)&varvalue) == FAILURE) 
    {
        // the variable does not already exist, return a global object
        // that will automatically set the value when it is updated
        return Global(name);
    }
    else
    {
        // we are in the happy situation that the variable exists, we turn
        // this value into a reference value, and return that
        return Global(name, *varvalue);
    }
}

/**
 *  End of namespace
 */
}

