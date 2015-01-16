/**
 *  Function.cpp
 *
 *  Implementation file for the Function class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Function returns a pointer to the class-entry of the Functor class
 *  @return zend_class_entry
 */
zend_class_entry *Function::entry()
{
    // and the actual class entry
    static zend_class_entry *entry = nullptr;
    
    // is the class entry already valid?
    if (entry) return entry;

    // construct functor object
    static std::unique_ptr<ClassBase> functor(new Class<Functor>("Functor"));
    
    // we need the TSRMLS variable
    TSRMLS_FETCH();

    // initialize the functor class
    return entry = functor->implementation()->initialize(functor.get(), "" TSRMLS_CC);
}

/**
 *  Constructor
 *  @param  function        The function to be wrapped
 */
Function::Function(const std::function<Php::Value(Php::Parameters&)> &function) : Value(Object(entry(), new Functor(function))) {}

/**
 *  End of namespace
 */
}

