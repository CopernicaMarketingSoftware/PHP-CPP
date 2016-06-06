#include <phpcpp.h>
#include "master.h"
#include "child.h"

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("returnobjecy", "1.0");
        
        // we have to class - master and child
        Php::Class<Master> master("master");
        Php::Class<Child> child("child");
        
        // the master class has one method - to return a child
        master.method<&Master::child>("child");
        
        // add all classes to the extension
        extension.add(master);
        extension.add(child);
        
        // return the extension
        return extension;
    }
}
