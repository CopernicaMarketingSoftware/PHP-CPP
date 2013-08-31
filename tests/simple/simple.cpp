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

static int my_plus(int a, int b)
{
    return a + b;
}

static std::string my_concat(std::string &a, std::string &b)
{
    return a + b;
}

Function f({"my_plus", my_plus});


// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        // create extension
        static PhpCpp::Extension extension("simple","1.0", {
            {"my_plus", my_plus},
            {"my_concat", my_concat}
        });
        
        // return the module entry
        return extension.getEntry();
    }
}

