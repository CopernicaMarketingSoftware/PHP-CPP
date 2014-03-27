/**
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

#include <string>
#include <iostream>
#include <phpcpp.h>

// Test includes
#include "include/ValueIterator.h"
#include "include/MyCustomClass.h"
#include "include/variables.h"




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
        
        // we are going to define a class
        Php::Class<TestBaseClass::MyCustomClass> customClass("TestBaseClass\\MyClass");
        
        // add methods to it
        customClass.method("myMethod", &TestBaseClass::MyCustomClass::myMethod, Php::Final, {});
        customClass.property("property1", "prop1");
        customClass.property("property2", "prop2", Php::Protected);

        // add the class to the extension
        extension.add(customClass);

        /**
         *  tests for Iterators
         * 
         */
        // add function to extension
        //extension.add("TestValueIterator\\loopValue", TestValueIterator::loopValue/*, {
        extension.add("TestValueIterator\\loopValue", TestValueIterator::loopValue);


        /**
         *  tests for variables
         * 
         */
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






        
        // return the extension module
        return extension;
    }
}
