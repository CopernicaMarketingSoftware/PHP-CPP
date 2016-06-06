/**
 *  functionnoparameters.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a function call without parameters.
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
 *  my_no_parameters_function()
 *  @return Php::Value
 */
Php::Value my_no_parameters_function()
{
    return "42";
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_function_no_parameters","1.0");
        
        // add function to extension
        extension.add<my_no_parameters_function>("my_no_parameters_function");
        
        // return the extension module
        return extension.module();
    }
}
