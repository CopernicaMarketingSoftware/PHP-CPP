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
 *  Call function with a number of parameters
 *  @param  name        Function name
 *  @param  argc        Number of parameters
 *  @param  argv        The parameters
 *  @return Value
 */
Value Environment::exec(const Value &name, int argc, zval ***params)
{
    // the return zval
    zval *retval = nullptr;
    
    // call the function
    if (call_user_function_ex(CG(function_table), NULL, name._val, &retval, argc, params, 1, NULL) != SUCCESS) return nullptr;
    
    // was a value returned?
    if (retval) return Value(retval);
    
    // no value was returned, return NULL
    return nullptr;
}

/**
 *  Call a function in PHP that does not have any parameters
 *  @param  name        Name of the function
 *  @return Value
 */
Value Environment::call(const Value &name)
{
    // call with zero parameters
    return exec(name, 0, NULL);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0)
{
    // array of parameters
    zval **params[1] = { &p0._val };

    // call the function
    return exec(name, 1, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1)
{
    // array of parameters
    zval **params[2] = { &p0._val, &p1._val };
    
    // call the function
    return exec(name, 2, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2)
{
    // array of parameters
    zval **params[3] = { &p0._val, &p1._val, &p2._val };
    
    // call the function
    return exec(name, 3, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3)
{
    // array of parameters
    zval **params[4] = { &p0._val, &p1._val, &p2._val, &p3._val };
    
    // call the function
    return exec(name, 4, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4)
{
    // array of parameters
    zval **params[5] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val };
    
    // call the function
    return exec(name, 5, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @param  p5          The sixth parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5)
{
    // array of parameters
    zval **params[6] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val, &p5._val };
    
    // call the function
    return exec(name, 6, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @param  p5          The sixth parameter
 *  @param  p6          The seventh parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6)
{
    // array of parameters
    zval **params[7] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val, &p5._val, &p6._val };
    
    // call the function
    return exec(name, 7, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @param  p5          The sixth parameter
 *  @param  p6          The seventh parameter
 *  @param  p7          The eight parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7)
{
    // array of parameters
    zval **params[8] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val, &p5._val, &p6._val, &p7._val };
    
    // call the function
    return exec(name, 8, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @param  p5          The sixth parameter
 *  @param  p6          The seventh parameter
 *  @param  p7          The eight parameter
 *  @param  p8          The nineth parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8)
{
    // array of parameters
    zval **params[9] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val, &p5._val, &p6._val, &p7._val, &p8._val };
    
    // call the function
    return exec(name, 9, params);
}

/**
 *  Call a function in PHP
 *  @param  name        Name of the function
 *  @param  p0          The first parameter
 *  @param  p1          The second parameter
 *  @param  p2          The third parameter
 *  @param  p3          The fourth parameter
 *  @param  p4          The fifth parameter
 *  @param  p5          The sixth parameter
 *  @param  p6          The seventh parameter
 *  @param  p7          The eight parameter
 *  @param  p8          The nineth parameter
 *  @param  p9          The tenth parameter
 *  @return Value
 */
Value Environment::call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8, Value p9)
{
    // array of parameters
    zval **params[10] = { &p0._val, &p1._val, &p2._val, &p3._val, &p4._val, &p5._val, &p6._val, &p7._val, &p8._val, &p9._val };
    
    // call the function
    return exec(name, 10, params);
}

/**
 *  End of namespace
 */
}

