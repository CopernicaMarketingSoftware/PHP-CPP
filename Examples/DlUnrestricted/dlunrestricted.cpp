/**
 *  functionvoid.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a void function call.
 */

/**
 *  Libraries used.
 */
#include <phpcpp.h>

/**
 *  Function to load every possible extension by pathname
 *
 *  It takes one argument: the filename of the PHP extension, and returns a 
 *  boolean to indicate whether the extension was correctly loaded.
 *
 *  This function goes further than the original PHP dl() fuction, because
 *  it does not check whether the passed in extension object is stored in the
 *  right directory. Literally every possible extension, also local ones 
 *  created by end users, can be loaded.
 *
 *  @param  params      Vector of parameters
 *  @return boolean
 */
Php::Value dl_unrestricted(Php::Parameters &params)
{
    // get extension name
    std::string pathname = params[0];
    
    // should it be opened persistently?
    bool persistent = params.size() > 1 ? params[1].boolValue() : false;
    
    // load the extension
    return Php::dl(pathname, persistent);
}
    
/**
 *  Switch to C context to ensure that the get_module() function
 *  is callable by C programs (which the Zend engine is)
 */
extern "C" {
    /**
     *  Startup function that is called by the Zend engine 
     *  to retrieve all information about the extension
     *  @return void*
     */
    PHPCPP_EXPORT void *get_module() {
        // create static instance of the extension object
        static Php::Extension myExtension("dl_unrestricted", "1.0");
        
        // the extension has one method
        myExtension.add<dl_unrestricted>("dl_unrestricted", {
            Php::ByVal("pathname", Php::Type::String)
        });
        
        // return the extension
        return myExtension;
    }
}

