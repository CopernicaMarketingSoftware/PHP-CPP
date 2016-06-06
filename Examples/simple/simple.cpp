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


Php::Value bubblesort(Php::Parameters &params)
{
    cout << "start bubblesort" << endl;
    
    // the array that was passed
    Php::Value array(params[0]);
    
    cout << "go return" << endl;
    
    return array;
    
    // size of the array
    int size = array.size();
    
    cout << "convert to native" << endl;
    
    int *x = new int[size];
    for (int i=0; i<size; i++) x[i] = array[i];
    
    cout << "converted" << endl;
    
    
    // outer loop
    for (int i=0; i<size; i++)
    {
        // left value
        int left = x[i];
//        cout << "left: " << left << endl;
        
        // inner loop
        for (int j=i+1; j<size; j++)
        {
            // right value
            int right = x[j];
            
            if (left < right) continue;
            
            // swap values
            x[i] = right;
            x[j] = left;
            left = right;
        }
    }
    
    cout << "algorithm end" << endl;
    
    Php::Value r;
    
    for (int i=0; i<size; i++) r[i] = x[i];
    
    
    delete[] x;
    
    // done
    return r;
}



/**
 *  Our own "my_plus" function that will be available in PHP
 *  @param  environment
 *  @param  params
 *  @return Value
 */
static Php::Value my_plus(Php::Parameters &params)
{
    Php::Value r(0);
    
    for (unsigned int i=0; i<params.size(); i++) r += params[i];
    
    return r;
    
    
//    int p1 = params[0];
//    int p2 = params[1];
//    return p1 + p2;
//    
//    cout << "global g1: " << env["g1"].stringValue() << endl;
//    cout << "global g2: " << env["g2"].stringValue() << endl;
//    
//    Php::Global g(env["g3"]);
//    
//    g = "zo kan het ook";
//    
//    string output = env.call("strtoupper","test in lowercase");
//    
//    cout << "output: " << output << endl;
//    
//    return p1 + p2;
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
//        cout << "A: " << _properties["a"] << endl;
//        cout << "Z: " << _properties["z"] << endl;
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
        extension.add<my_plus>("my_plus", {
            Php::ByVal("a", Php::Type::Numeric),
            Php::ByVal("b", Php::Type::Numeric),
            Php::ByVal("c", "MyClass"),
            Php::ByRef("d", Php::Type::String)
        });
        
        extension.add<bubblesort>("bubblesort");
        
        // define classes
        Php::Class<MyCustomClass> myCustomClass("my_class");
        myCustomClass.method<&MyCustomClass::myMethod>("mymethod");
        myCustomClass.method<&MyCustomClass::__construct>("__construct");
        
        // add to extension
        extension.add(myCustomClass);
        
        // return the module entry
        return extension.module();
    }
}

