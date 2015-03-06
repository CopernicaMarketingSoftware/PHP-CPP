/**
 *  Functor.cpp
 *
 *  Implementation file for the functor class
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
 *  The classentry
 *  @var zend_class_entry
 */
zend_class_entry *Functor::_entry = nullptr;

/**
 *  Initialize the class
 *  @param  tsrmls
 */
void Functor::initialize(TSRMLS_D)
{
    // leap out if the class entry is already set
    if (_entry) return;
    
    // construct functor object
    static std::unique_ptr<ClassBase> functor(new Class<Functor>("PhpCpp::Functor"));
    
    // initialize the functor class
    _entry = functor->implementation()->initialize(functor.get(), "" TSRMLS_CC);
}

/**
 *  Shutdown the class
 *  @param  tsrmls
 */
void Functor::shutdown(TSRMLS_D)
{
    // we forget the entry
    _entry = nullptr;
}
    
/**
 *  End of namespace
 */
}

