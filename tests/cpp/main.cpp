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
#include "h/ini_entries.h"




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

        customClass.property("CONSTANT1", "some string", Php::Const);
        customClass.property("EXP",       2.718281828459, Php::Const);
        customClass.property("CONSTANT2", -2582341, Php::Const);
        customClass.property("CONSTANT3", true, Php::Const);

        customClass.property("StatProp1", "some string", Php::Static);
        customClass.property("Exp",       2.718281828459, Php::Static);
        customClass.property("StatProp2", -2582341, Php::Static);
        customClass.property("StatProp3", true, Php::Static);

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
        extension.add("TestVariables\\fnFromUserSpace2",  TestVariables::fnFromUserSpace2);
        extension.add("TestVariables\\fnCallback",        TestVariables::fnCallback);
        extension.add("TestVariables\\test_HashMember_1", TestVariables::test_HashMember_1);
        extension.add("TestVariables\\test_HashMember_2", TestVariables::test_HashMember_2);
        extension.add("TestVariables\\test_HashMember_3", TestVariables::test_HashMember_3);
        extension.add("TestVariables\\test_HashMember_4", TestVariables::test_HashMember_4);
        extension.add("TestVariables\\getCookie",         TestVariables::getCookie);
        extension.add("TestVariables\\get_post",          TestVariables::get_post);
        extension.add("TestVariables\\post_raw1",         TestVariables::post_raw1);
        extension.add("TestVariables\\post_raw2",         TestVariables::post_raw2);
        extension.add("TestVariables\\test_env",          TestVariables::test_env);
        extension.add("TestVariables\\test_compare1",     TestVariables::test_compare1);
        extension.add("TestVariables\\test_compare2",     TestVariables::test_compare2);
        extension.add("TestVariables\\test_constants",     TestVariables::test_constants);

        


        // A sample class, with methods to cast objects to scalars
        Php::Class<TestVariables::Obj2Scalar> cObj2Scalar("TestVariables\\Obj2Scalar");
        extension.add(std::move(cObj2Scalar));


        /**
         *  tests ini entries
         * 
         */
        extension
            .add(Php::Ini("ini1", "valIni1"))
            .add(Php::Ini("ini2", "valIni2", "OrigValIni2"))
            .add(Php::Ini("ini3", "valIni3", "OrigValIni3", Php::Ini::System))
            .add(Php::Ini("ini4", true,      false, Php::Ini::Place::User))
            .add(Php::Ini("ini5",            false));

        Php::Ini ini6("ini6", 55, 11);
        extension
            .add(ini6)
            .add(Php::Ini("ini7", 74,5));

        Php::Ini ini8("ini8", 3.1415926, 6.2831852);
        Php::Ini ini9("ini9", 2.7182818, 5.4365636, Php::Ini::User);
        //extension.add(Php::Ini("ini9", 0.333333, 0.777777, Php::Ini::Perdir));

        extension.add(ini8);
        extension.add(std::move(ini9));

        extension.add("TestIniEntries\\iniTest1", TestIniEntries::iniTest1);

        extension.onStartup([](){
            // Retrieve a value at boot extension
            TestIniEntries::ini6val = Php::ini_get("ini6");
        });
         





        
        // return the extension module
        return extension;
    }
}
