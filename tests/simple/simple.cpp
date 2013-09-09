/**
 *  Simple.h
 *
 *  A very simple extension that does almost nothing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include <string>
#include <iostream>
#include <phpcpp.h>

/**
 *  Namespace to use
 */
using namespace std;

static Php::Value my_plus(Php::Parameters &params)
{
    string p1 = params[0];
    string p2 = params[1];
    
    return p1 + p2;
}

class MyCustomFunction : public Php::Function
{
    
    
};

// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        cout << "a" << endl;
        
        // create extension
        static Php::Extension extension("simple","1.0");

        cout << "b" << endl;
        
        // define the functions
        extension.add("my_plus", my_plus);
//        extension.add("my_concat", my_concat);
        extension.add("my_custom", MyCustomFunction());

        cout << "c" << endl;
        
        // define classes
//        extension.add("my_class", MyCustomClass());
        
        // return the module entry
        return extension.module();
    }
}

