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
 *  Function to test the Php::include_once() call
 *  @param      &params
 */
void my_include_once(Php::Parameters &params)
{
    // the string
    Php::Value path = params[0];
    
    // do the call
    Php::include_once(path.stringValue());
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("include_once","1.0");
        
        // add function to extension
        extension.add<my_include_once>("my_include_once", {
            Php::ByVal("path", Php::Type::String),
        });
        
        // return the extension module
        return extension.module();
    }
}
