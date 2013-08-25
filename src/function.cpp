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
    // one reference to the callable
    _refcount = new int(1);
    _callable = new Callable(name, arguments);
}

/**
 *  Remove one reference
 */
void Function::cleanup()
{
    // decrease number of references
    (*_refcount)--;
    
    // leap out if there are still other references
    if (*_refcount > 0) return;
    
    // release memory
    delete _refcount;
    delete _callable;
}

/**
 *  End of namespace
 */
}


