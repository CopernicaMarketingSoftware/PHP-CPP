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
 */
void Functor::initialize()
{
    // leap out if the class entry is already set
    if (_entry) return;

    // construct functor object
    static std::unique_ptr<ClassBase> functor(new Class<Functor>("PhpCpp::Functor"));

    // initialize the functor class
    _entry = functor->implementation()->initialize(functor.get(), "");
}

/**
 *  Shutdown the class
 */
void Functor::shutdown()
{
    // we forget the entry
    _entry = nullptr;
}

/**
 *  End of namespace
 */
}

