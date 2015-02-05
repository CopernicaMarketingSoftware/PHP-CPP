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
 *  Constructor
 *  @param  function        The function to be wrapped
 */
Function::Function(const std::function<Php::Value(Php::Parameters&)> &function) : Value(Object(Functor::entry(), new Functor(function))) {}

/**
 *  End of namespace
 */
}

