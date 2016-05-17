/**
 *  Globals.cpp
 *
 *  Implementation of the globals class
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
 *  Get access to the globals single instance
 *  @return Globals
 */
Globals &Globals::instance()
{
    static Globals globals;
    return globals;
}

/**
 *  The one and only instance
 *  @var    Globals
 */
Globals &GLOBALS = Globals::instance();

/**
 *  Get access to a global variable
 *  @param  name
 *  @return Global
 */
Global Globals::operator[](const char *name)
{
    // we need the TSRMLS variable
    TSRMLS_FETCH();

    // retrieve the variable (if it exists)
    auto *varvalue = zend_hash_find(&EG(symbol_table), zend_string_init(name, ::strlen(name), 0));

    // check if the variable already exists
    if (!varvalue)
    {
        // the variable does not already exist, return a global object
        // that will automatically set the value when it is updated
        return Global(name);
    }
    else
    {
        // we are in the happy situation that the variable exists, we turn
        // this value into a reference value, and return that
        return Global(name, varvalue);
    }
}

/**
 *  Get access to a global variable
 *  @param  name
 *  @return Global
 */
Global Globals::operator[](const std::string &name)
{
    // we need the TSRMLS variable
    TSRMLS_FETCH();

    // retrieve the variable (if it exists)
    auto *varvalue = zend_hash_find(&EG(symbol_table), zend_string_init(name.data(), name.size(), 0));

    // check if the variable already exists
    if (!varvalue)
    {
        // the variable does not already exist, return a global object
        // that will automatically set the value when it is updated
        return Global(name);
    }
    else
    {
        // we are in the happy situation that the variable exists, we turn
        // this value into a reference value, and return that
        return Global(name, varvalue);
    }
}

/**
 *  End of namespace
 */
}

