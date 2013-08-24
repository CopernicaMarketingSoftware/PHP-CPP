/**
 *  Simple.h
 *
 *  A very simple extension that does almost nothing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include <phpcpp.h>

/**
 *  Override the extension class
 */
class SimpleExtension : public PhpCpp::Extension
{
public:
    /**
     *  Constructor
     */
    SimpleExtension() : Extension(
        "simple", 
        "1.0", 
        "Emiel Bruijntjes <emiel.bruijntjes@copernica.com>", 
        "http://www.copernica.com", 
        "Copyright 2013 Copernica BV")
    {
    }
};

extern "C" {

// create the object for the PHP extension
PHP_CPP_EXTENSION(SimpleExtension);

}
