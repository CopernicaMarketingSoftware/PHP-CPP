/**
 *  Simple.h
 *
 *  A very simple extension that does almost nothing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include <phpcpp.h>
#include <iostream>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  Override the extension class
 */
class SimpleExtension : public PhpCpp::Extension
{
public:
    /**
     *  Constructor
     */
    SimpleExtension() : Extension("simple", "1.0")
    {
    }
    
    virtual bool initialize()
    {
        cout << "initialize" << endl;
        return true;
    }
    
    virtual bool finalize()
    {
        cout << "finalize" << endl;
        return true;
    }
    
};

// create the object for the PHP extension
PHP_CPP_EXTENSION(SimpleExtension);
