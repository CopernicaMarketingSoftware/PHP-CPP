/**
 *  Function.cpp
 *
 *  Implementation for the function class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Constructor
 *  @param  name        Name of the function
 *  @param  arguments   The arguments that can be passed to the function
 */
Function::Function(const std::string &name, const std::initializer_list<Argument> &arguments)
{
    // create callable object
    _callable = new Callable(name, arguments);
}

/**
 *  Destructor
 */
Function::~Function()
{
    if (_callable) delete _callable;
}

/**
 *  End of namespace
 */
}


