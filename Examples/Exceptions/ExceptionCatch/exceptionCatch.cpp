/**
 *  exception.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a C++ function that 
 *  takes a callback function as parameter, and handles the 
 *  exception thrown by the callback function.
 */

/**
 *  Libraries used.
 */
#include <phpcpp.h>
#include <iostream>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  my_catch_exception_function()
 *  Catches the exception thrown by the PHP callback function.
 *  @param      Php::Parameters
 */
void my_catch_exception_function(Php::Parameters &params)
{
    // the first parameter should be a callback
    Php::Value callback = params[0];

    // this must be a callable type
    if (!callback.isCallable()) throw Php::Exception("Parameter 0 is not a function");

    // we're going to call a function that could throw an exception, start a 
    // try-catch block to deal with that
    try
    {
        // call the function
        callback("some","example","parameters");
    }
    catch (Php::Exception &exception)
    {
        // handle the exception that was thrown from PHP space
        std::cout << "exception caught in CPP code" << std::endl;
    }
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_exception_catch","1.0");
        
        // add function to extension
        extension.add<my_catch_exception_function>("my_catch_exception_function");
        
        // return the extension module
        return extension.module();
    }
}
