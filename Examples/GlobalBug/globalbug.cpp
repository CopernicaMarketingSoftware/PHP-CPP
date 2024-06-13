/**
 *  globalbug.cpp
 *  @author Charlie Bouthoorn <cbouthoorn19@gmail.com>
 *
 *  An example file which shows a former bug in globals
 */

#define NAME "globalbug"
#define VERSION "1.0"

#include <iostream>
#include <phpcpp.h>

/**
 *  process_globals()
 *
 *  This function reads and modifies global variables
 */
void process_globals()
{
    // Create a global array
    Php::GLOBALS["array"] = Php::Array();

    // Get this global back
    Php::Global array = Php::GLOBALS["array"];

    // Store a value in a member field
    // NOTE: This is where the bug was. Changing this value didn't
    //       result in the global being updated.
    array["member"] = 123;

    // For comparison: These two answers should be the same
    std::cout << array["member"] << std::endl;
    std::cout << Php::GLOBALS["array"]["member"] << std::endl;
}

// Symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension(NAME, VERSION);

        // add function to extension
        extension.add<process_globals>("process_globals");


        return extension.module();
    }
}
