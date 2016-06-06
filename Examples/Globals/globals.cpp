/**
 *  functionvoid.cpp
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 * 
 *  An example file to show how global variables can be accessed
 */

/**
 *  Libraries used.
 */
#include <iostream>
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  process_globals()
 * 
 *  This function reads and modifies global variables
 */
Php::Value process_globals()
{
    // all global variables can be accessed via the Php::GLOBALS variable,
    // which is more or less the same as the PHP $_GLOBALS variable
    
    // set a global variable
    Php::GLOBALS["a"] = 1;
    
    // increment a global variable
    Php::GLOBALS["b"] += 1;
    
    // set a global variable to be an array
    Php::GLOBALS["c"] = Php::Array();
    
    // add a member to an array
    Php::GLOBALS["c"]["member"] = 123;
    
    // and increment it
    Php::GLOBALS["c"]["member"] += 77;
    
    // change value e
    Php::GLOBALS["e"] = Php::GLOBALS["e"][0]("hello");
    
    // if a global variable holds a function, we can call it
    return Php::GLOBALS["d"](1,2,3);
}

// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("globals","1.0");
        
        // add function to extension
        extension.add<process_globals>("process_globals");
        
        // return the extension module
        return extension.module();
    }
}
