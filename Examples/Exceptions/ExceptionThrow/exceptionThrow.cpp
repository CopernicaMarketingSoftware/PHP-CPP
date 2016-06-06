/**
 *  exception.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a C++ function that 
 *  throws an exception, which can be caught by PHP.
 *  
 */

/**
 *  Libraries used.
 */
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  my_throw_exception_function()
 *  Throws an exception which should be caught by PHP.
 */
void my_throw_exception_function()
{
    throw Php::Exception("I threw an exception in my_throw_exception_function()");
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_exception_throw","1.0");
        
        // add function to extension
        extension.add<my_throw_exception_function>("my_throw_exception_function");
        
        // return the extension module
        return extension.module();
    }
}
