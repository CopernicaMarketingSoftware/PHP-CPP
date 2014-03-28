/**
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

#include <string>
#include <iostream>
#include <phpcpp.h>

// Test includes
#include "h/ValueIterator.h"
#include "h/Classes_and_objects.h"
#include "h/variables.h"




// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("extension_for_tests","0.1");
        
        // build an interface
        //Php::Interface interface("MyInterface");
        
        // add methods to the interface
        //interface.method("method1");
        //interface.method("method2");
        
        // add the interface to the extension
        //extension.add(interface);

        


        /**
         *  Classes and objects
         * 
         */
        // we are going to define a class
        Php::Class<TestBaseClass::MyCustomClass> customClass("TestBaseClass\\MyClass");
        
        // add methods to it
        customClass.method("myMethod", &TestBaseClass::MyCustomClass::myMethod, Php::Final, {});
        customClass.property("property1", "prop1");
        customClass.property("property2", "prop2", Php::Protected);

        // add the class to the extension
        extension.add(customClass);

        // Comparable
        extension.add( Php::Class<TestBaseClass::Comparable>("TestBaseClass\\Comparable") );


        // test static functions
        //
        // description of the class so that PHP knows which methods are accessible
        Php::Class<TestBaseClass::testStaticPubClass> ClassWithStatic("TestBaseClass\\ClassWithStatic");
        // register the testStaticPubClass::staticMethod to be a static method callable from PHP
        ClassWithStatic.method("static1", &TestBaseClass::testStaticPubClass::staticMethod);
        // regular functions have the same signatures as static methods. So nothing forbids you to register a normal function as static method too
        ClassWithStatic.method("static2", TestBaseClass::testStaticRegFunc);
        // and even static methods from completely different classes have the same function signature and can thus be registered
        ClassWithStatic.method("static3", &TestBaseClass::testStaticPrivClass::staticMethod);
        // add the class to the extension
        extension.add(std::move(ClassWithStatic));
        // In fact, because a static method has the same signature
        // as a regular function, you can also register static
        // C++ methods as regular global PHP functions
        extension.add("TestBaseClass\\staticFun1", &TestBaseClass::testStaticPrivClass::staticMethod);




        /**
         *  tests for Iterators
         * 
         */
        // add function to extension
        //extension.add("TestValueIterator\\loopValue", TestValueIterator::loopValue/*, {
        extension.add("TestValueIterator\\loopValue", TestValueIterator::loopValue);
        extension.add("TestValueIterator\\loopArray", TestValueIterator::loopArray);


        /**
         *  tests for variables
         * 
         */
        // create a nested namespace
        extension.add("TestVariables\\process_globals",   TestVariables::process_globals);
        extension.add("TestVariables\\get_complex_array", TestVariables::get_complex_array);
        extension.add("TestVariables\\value_types",       TestVariables::value_types);
        extension.add("TestVariables\\scalar_store",      TestVariables::scalar_store);
        extension.add("TestVariables\\value_casting",     TestVariables::value_casting);
        extension.add("TestVariables\\value_cast2double", TestVariables::value_cast2double);
        extension.add("TestVariables\\value_cast2str",    TestVariables::value_cast2str);
        extension.add("TestVariables\\overloaded_op",     TestVariables::overloaded_op);
        extension.add("TestVariables\\value_arrays",      TestVariables::value_arrays);
        extension.add("TestVariables\\value_object1",     TestVariables::value_object1);
        extension.add("TestVariables\\value_object2",     TestVariables::value_object2);
        extension.add("TestVariables\\fnFromUserSpace",   TestVariables::fnFromUserSpace);
        extension.add("TestVariables\\fnFromUserSpace2",   TestVariables::fnFromUserSpace2);
        extension.add("TestVariables\\fnCallback",        TestVariables::fnCallback);
        


        // A sample class, with methods to cast objects to scalars
        Php::Class<TestVariables::Obj2Scalar> cObj2Scalar("TestVariables\\Obj2Scalar");
        extension.add(std::move(cObj2Scalar));






        
        // return the extension module
        return extension;
    }
}
