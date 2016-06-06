/**
 *  callphpfunction.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a php function call in C++.
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
 *  call_php_function()
 *  Calls a function in PHP space.
 *  @param      &params
 *  @return     Php::Value
 */
Php::Value call_php_function(Php::Parameters &params)
{
    // check whether the parameter is callable
    if (!params[0].isCallable()) throw Php::Exception("Not a callable type.");
        
    // perform the callback
    return params[0](1,2,3);
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("call_php_function","1.0");
        
        // add function to extension
        extension.add<call_php_function>("call_php_function", {
            Php::ByVal("addFunc", Php::Type::Callable),
            Php::ByVal("x", Php::Type::Numeric)
            });
        
        // return the extension module
        return extension.module();
    }
}
