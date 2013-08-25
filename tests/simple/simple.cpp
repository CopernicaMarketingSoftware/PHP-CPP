/**
 *  Simple.h
 *
 *  A very simple extension that does almost nothing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include <string>
#include <phpcpp.h>
#include <iostream>

/**
 *  Namespace to use
 */
using namespace std;

/**
 *  Override the request class
 */
class SimpleRequest : public PhpCpp::Request
{
public:
    SimpleRequest(PhpCpp::Extension *extension) : PhpCpp::Request(extension)
    {
    }

    virtual bool initialize()
    {
        cout << "Request::initialize" << endl;
        return true;
    }
    
    virtual bool finalize()
    {
        cout << "Request::finalize" << endl;
        return true;
    }
};

/**
 *  Override the extension class
 */
class SimpleExtension : public PhpCpp::Extension
{
public:
    /**
     *  Constructor
     */
    SimpleExtension() : Extension("simple", "1.0", {
        PhpCpp::Function("hallo")
    })
    {
    }
    
    virtual bool initialize()
    {
        cout << "Extension::initialize" << endl;
        return true;
    }
    
    virtual bool finalize()
    {
        cout << "Extension::finalize" << endl;
        return true;
    }
    
    virtual PhpCpp::Request *request()
    {
        return new SimpleRequest(this);
    }
};




// create the object for the PHP extension
PHP_CPP_EXTENSION(SimpleExtension);
