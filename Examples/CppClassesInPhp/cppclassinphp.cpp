/**
 *  cppclassinphp.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

#include "includeMyCustomClass.h"
/**
 *  Namespace to use
 */
using namespace std;

class MyCustomClass : public Php::Base
{
private:
    int _x;
    
public:
    MyCustomClass()
    {
        _x = 3;
        std::cout << "MyCustomClass::MyCustomClass" << std::endl;
        std::cout << _x << std::endl;
    }
    
    virtual ~MyCustomClass()
    {
        std::cout << "MyCustomClass::~MyCustomClass" << std::endl;
    }

    virtual void __construct()
    {
        std::cout << "MyCustomClass::__construct" << std::endl;
    }

    virtual void __destruct()
    {
        std::cout << "MyCustomClass::__destruct" << std::endl;
    }
    
    void myMethod(Php::Parameters &params)
    {
        std::cout << "myMethod is called." << std::endl;
        std::cout << "_x: " << _x << std::endl;
        _x = params[0];
        std::cout << "New _x" << _x << std::endl;
    }
};

void myFunction(Php::Parameters &params)
{
    std::cout << "regular function" << std::endl;
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("Cpp_classes_in_php","1.0");
        
        // add custom function
        extension.add("myFunction", myFunction, { });
        
        // we are going to define a class
        Php::Class<MyCustomClass> customClass("MyClass");
        
        // add methods to it
        // @todo support setting parameter properties
        customClass.add("myMethod", &MyCustomClass::myMethod, {});
        
        // add the class to the extension
        extension.add(customClass);
                
        // return the extension module
        return extension.module();
    }
}
