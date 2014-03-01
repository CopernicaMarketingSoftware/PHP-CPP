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
        // check number of parameters
        if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");
        
        std::cout << "myMethod is called." << std::endl;
        _x = params[0];
    }
};

// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("Cpp_classes_in_php","1.0");
        
        // build an interface
        Php::Interface interface("MyInterface");
        
        // add methods to the interface
        interface.method("method1");
        interface.method("method2");
        
        // add the interface to the extension
        extension.add(interface);
        
        // we are going to define a class
        Php::Class<MyCustomClass> customClass("MyClass");
        
        // add methods to it
        customClass.method("myMethod", &MyCustomClass::myMethod, Php::Final, {});
        customClass.method("myMethod2", &MyCustomClass::myMethod);
        customClass.property("property1", "prop1");
        customClass.property("property2", "prop2", Php::Protected);
        customClass.method("myAbstract");
        
        // add the class to the extension
        extension.add(customClass);
        
        // return the extension module
        return extension.module();
    }
}
