/**
 *  functionwithparameters.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a function call with parameters.
 */

/**
 *  Default Cpp libraries
 */
 
#include <string>
#include <iostream>

/**
 *  Our own library.
 */
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  my_with_undefined_parameters_function()
 *  @param  Php:Parameters  the given parameters
 */
void my_with_undefined_parameters_function(Php::Parameters &params)
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        cout << "Parameter " << i << ": " << params[i] << endl;
    }
}

/**
 *  my_with_defined_parameters_function()
 *  @param  Php::Parameters     the given parameters
 *  @return Php::Value          Param[0] and Param[1] added
 */
Php::Value my_with_defined_parameters_function(Php::Parameters &params)
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        cout << "Parameter " << i << ": " << params[i] << endl;
    }
    
    return params[0] + params[1];
}

/**
 *  This functions receives a reference to a variable. When the variable is altered,
 *  so is the value in the php script.
 *  my_with_defined_parameters_reference_function()
 *  @param  Php::Parameters     the given parameters
 */
void my_with_defined_parameters_reference_function(Php::Parameters &params)
{
    params[0] = "I changed!";
}

/**
 *  my_with_defined_parameters_reference_function()
 *  @param  Php::Parameters     the given parameters
 */
void my_with_defined_array_parameters_function(Php::Parameters &params)
{
    for (int i = 0; i < params[0].size(); i++)
    {
        cout << "The array: " << params[0][i] << endl;
    }
}

/**
 *  my_with_defined_object_parameters_function()
 *  The use of objects is not yet implemented.
 *  @param Php::Parameters      the given parameters
 */
void my_with_defined_object_parameters_function(Php::Parameters &params)
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        cout << params[i] << endl;
    }
}

// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_function_with_parameters","1.0");
        
        // add function, with undefined parameters, to extension
        extension.add<my_with_undefined_parameters_function>("my_with_undefined_parameters_function");
        
        // add function, with defined numeric parameters, to extension
        extension.add<my_with_defined_parameters_function>("my_with_defined_parameters_function", {
            Php::ByVal("x", Php::Type::Numeric),
            Php::ByVal("y", Php::Type::Numeric)
            });
        
        // add function, with defined parameter by reference, to extension
        extension.add<my_with_defined_parameters_reference_function>("my_with_defined_parameters_reference_function", {
            Php::ByRef("string", Php::Type::String)
            });
        
        // add function, with defined array parameter, to extension
        extension.add<my_with_defined_array_parameters_function>("my_with_defined_array_parameters_function", {
            Php::ByVal("array", Php::Type::Array)
            });
        
        // add function, with defined object parameter, to extension
        extension.add<my_with_defined_object_parameters_function>("my_with_defined_object_parameters_function", {
            Php::ByVal("myClassObjVar", "MyPhpClass")
            });
        
        // return the extension module
        return extension.module();
    }
}
