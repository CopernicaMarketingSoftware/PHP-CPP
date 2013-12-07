/**
 *  extension.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of an extension.
 */
// library include
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_simple_extension","1.0");
        
        // return the extension module
        return extension.module();
    }
}
