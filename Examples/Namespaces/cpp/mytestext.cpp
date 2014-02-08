/**
 *  mytestext.cpp
 * 
 *  An example showing how to set PHP-namespace in PHP-CPP
 *  see README.md
 */

#include <phpcpp.h>

/**
 * class NamespaceExample\TestClass
 */
class MyTestClass : public Php::Base
{
public:
    MyTestClass() {}
    virtual ~MyTestClass() {}
    virtual void __construct() {}
    virtual void __destruct() {}

    void myMethod() {}
};


/**
 * function NamespaceExample\TestFunc
 * @param void
 * @return string
 */
Php::Value MyTestFunc() {
    return "I'm is TestFunc";
}


// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("NamespaceExampleExt","0.1");
        
        // add the custom class ot the extension
        extension.add(
            // set PHP-namespace `NamespaceExample`
            "NamespaceExample\\TestClass", 
            Php::Class<MyTestClass>({
                Php::Public("myMethod", Php::Method<MyTestClass>(&MyTestClass::myMethod)),
            })
        );

        // set PHP-namespace `NamespaceExample`
        extension.add("NamespaceExample\\TestFunc", MyTestFunc);
                
        // return the extension module
        return extension.module();
    }
}
