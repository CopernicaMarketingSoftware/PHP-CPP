/**
 *	functionwithparameters.cpp
 * 	@Author Jasper van Eck
 * 
 * 	An example file to show the working of a function call with parameters.
 */

/**
 * 	Libraries used.
 */
#include <iostream>
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

/**
 * 	my_with_undefined_parameters_function()
 * 	@param	Php:Parameters	the given parameters
 */
void my_with_undefined_parameters_function(Php::Parameters &params)
{
	for (unsigned int i = 0; i < params.size(); i++)
	{
		cout << "Parameter " << i << ": " << params[i] << endl;
	}
}

/**
 * 	my_with_defined_parameters_function()
 * 	@param	Php::Parameters		the given parameters
 * 	@return Php::Value			Param[0] and Param[1] added
 */
Php::Value my_with_defined_parameters_function(Php::Parameters &params)
{
	for (unsigned int i = 0; i < params.size(); i++)
	{
		cout << "Parameter " << i << ": " << params[i] << endl;
	}
	
	return params[0] + params[1];
}

// Symbols are exported according to the "C" language
extern "C" 
{
	// export the "get_module" function that will be called by the Zend engine
	PHPCPP_EXPORT void *get_module()
	{
		// create extension
        static Php::Extension extension("my_function_with_parameters","1.0");
        
        // add function to extension
        extension.add("my_with_undefined_parameters_function", my_with_undefined_parameters_function);
		
		extension.add("my_with_defined_parameters_function", my_with_defined_parameters_function, {
			Php::ByVal("x", Php::numericType),
            Php::ByVal("y", Php::numericType)
			});
		
		// return the extension module
		return extension.module();
	}
}
