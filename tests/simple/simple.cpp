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

/**
 *  Our own "my_plus" function that will be available in PHP
 *  @param  environment
 *  @param  params
 *  @return Value
 */
static Php::Value my_plus(Php::Environment &env, Php::Parameters &params)
{
    int p1 = params[0];
    int p2 = params[1];
    return p1 + p2;
    
    cout << "global g1: " << env["g1"].stringValue() << endl;
    cout << "global g2: " << env["g2"].stringValue() << endl;
    
    Php::Global g(env["g3"]);
    
    g = "zo kan het ook";
    
    string output = env.call("strtoupper","test in lowercase");
    
    cout << "output: " << output << endl;
    
    return p1 + p2;
}

/**
 *  Custom class that will be available in PHP
 */
class MyCustomClass : public Php::Base
{
private:
    int _x;
    
public:
    MyCustomClass()
    {
        _x = 3;
        cout << "MyCustomClass::MyCustomClass" << endl;
        cout << this << endl;
        cout << _x << endl;
    }
    
    virtual ~MyCustomClass()
    {
        cout << "MyCustomClass::~MyCustomClass" << endl;
    }

    virtual void __construct()
    {
        cout << "MyCustomClass::__construct" << endl;
    }

    virtual void __destruct()
    {
        cout << "MyCustomClass::__destruct" << endl;
    }
    
    void myMethod(Php::Parameters &params)
    {
        cout << "myMethod GETS CALLED!!!!" << endl;
        cout << this << endl;
        cout << _x << endl;
        
    }
};

// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        // create extension
        static Php::Extension extension("simple","1.0");

        // define the functions
        extension.add("my_plus", my_plus, {
            Php::ByVal("a", Php::longType),
            Php::ByVal("b", Php::longType)
//            Php::ByVal("c", "MyClass"),
//            Php::ByRef("d", Php::stringType)
        });
        
        Php::Method<MyCustomClass> x(&MyCustomClass::myMethod);
        
        // define classes
        extension.add("my_class", Php::Class<MyCustomClass>({
            Php::Public("a", 123),
            Php::Protected("b", "abc"),
            Php::Protected("myMethod", Php::Method<MyCustomClass>(&MyCustomClass::myMethod)),
            Php::Public("__construct", Php::Method<MyCustomClass>(&MyCustomClass::__construct))
        }));
        
        // return the module entry
        return extension.module();
    }
}

