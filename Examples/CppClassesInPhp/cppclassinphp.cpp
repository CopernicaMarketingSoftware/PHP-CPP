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

class MyCustomClass : public Php::Base // , public Php::Countable
{
private:
    int _x = 3;
    
public:
    MyCustomClass()
    {
        std::cout << "MyCustomClass::MyCustomClass()" << std::endl;
    }

    MyCustomClass(int value) : _x(value)
    {
        std::cout << "MyCustomClass::MyCustomClass(" << value << ")" << std::endl;
    }

    MyCustomClass(const MyCustomClass &that)
    {
        std::cout << "MyCustomClass::MyCustomClass copy constructor" << std::endl;
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
    
    virtual Php::Value count() //override
    {
        return 33;
    }

    void loop(Php::Parameters &params)
    {
        std::cout << "Array/Object contains " << params[0].size() << " items" << std::endl;
        auto m = params[0].mapValue();

        std::cout << "map contains " << m.size() << " items" << std::endl;
        for(auto &i: m) {
            std::cout << "key: " << i.first << " \t\tval: " << i.second << std::endl;
        }
    }
    
    Php::Value myMethod(Php::Parameters &params)
    {
        // check number of parameters
        if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");
        
        std::cout << "myMethod is called for object " << _x << std::endl;

        std::cout << "property1: " << property("property1") << std::endl;


        return Php::Object("MyClass", new MyCustomClass(100));


        return false;


        // create a new PHP DateTime object representing the current time
        Php::Object now("DateTime", "now");

        // show it
        std::cout << "current time: " << now.call("format", "Y-m-d H:i:s") << std::endl;
        
        std::cout << "construct " << params[0] << std::endl;

        // construct a new class
        Php::Object obj(params[0]);

        std::cout << "return " << params[0] << std::endl;
        
        
        
        
        // return it
        return obj;
        
        
   //     std::cout << "get property1 " << value()["property1"] << std::endl;
   //     
   //     // set it to something else
   //     value().set("property1", "new value");
   //
   //     std::cout << "get property1 " << value()["property1"] << std::endl;
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
        customClass.method<&MyCustomClass::myMethod>("myMethod", Php::Final, {});
        customClass.method<&MyCustomClass::myMethod>("myMethod2");
        customClass.property("property1", "prop1");
        customClass.property("property2", "prop2", Php::Protected);

        customClass.method<&MyCustomClass::loop>("loopArray", {
            Php::ByVal("arr", Php::Type::Array)
        });
        customClass.method<&MyCustomClass::loop>("loopObject", {
            Php::ByVal("obj", Php::Type::Object)
        });
        
        // add the class to the extension
        extension.add(customClass);
        
        // return the extension module
        return extension;
    }
}
