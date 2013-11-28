/**
 *	functionvoid.cpp
 * 	@Author Jasper van Eck
 * 
 * 	An example file to show the working of a void fucntion call.
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
 * 	my_function_void()
 */
void my_function_void()
{
	cout << "In my_function_void()" << endl;
}


// Symbols are exported according to the "C" language
extern "C" 
{
	// export the "get_module" function that will be called by the Zend engine
	PHPCPP_EXPORT void *get_module()
	{
		// create extension
        static Php::Extension extension("my_function_void","1.0");
        
        // add function to extension
        extension.add("my_void_function", my_function_void);
		
		// return the extension module
		return extension.module();
	}
}
