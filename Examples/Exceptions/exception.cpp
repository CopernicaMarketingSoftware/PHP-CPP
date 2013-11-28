/**
 *	exception.cpp
 * 	@author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 * 	An example file to show the working of a function which throws an exception.
 * 	
 * 	Exceptions haven't been implemented yet. 
 * 	Therefore this example is not yet a working one.
 */

/**
 * 	Libraries used.
 */
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

/**
 * 	my_throw_exception_function()
 * 	Throws an exception.
 * 	@return Php::Value
 */
Php::Value my_throw_exception_function()
{
	throw Php::Exception("I threw an exception in my_throw_exception_function()");
	return "42";
}


// Symbols are exported according to the "C" language
extern "C" 
{
	// export the "get_module" function that will be called by the Zend engine
	PHPCPP_EXPORT void *get_module()
	{
		// create extension
        static Php::Extension extension("my_function_throw_exception","1.0");
        
        // add function to extension
        extension.add("my_throw_exception_function", my_throw_exception_function);
		
		// return the extension module
		return extension.module();
	}
}
