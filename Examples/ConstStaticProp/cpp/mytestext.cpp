/**
 *  cppclassinphp.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 *
 *  An example file to show the working of using a C++ class in PHP.
 */

#include "mytestext.h"
/**
 *  Namespace to use
 */
using namespace std;

class MyTestExt : public Php::Base
{

public:
    MyTestExt() {}

    virtual ~MyTestExt() {}

    virtual void __construct() {}

};



// Symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("my_test_ext","0.1a");

        // add the custom class ot the extension
        extension.add(
            "MyTestClass",
            Php::Class<MyTestExt>({

                // Private PHP constructor! You can't instance object of MyTestClass
                Php::Private("__construct", Php::Method<MyTestExt>(&MyTestExt::__construct)),

                Php::Const("version", "v0.01-alpha"),
                Php::Const("PI", 3.14159265),
                Php::Const("IMISNULL"),
            })
        );

        // return the extension module
        return extension.module();
    }
}
