/**
 *  Argument.cpp
 *
 *  Implementation for the Argument class
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
 *  Constructor if this argument should be an instance of a certain class
 *  @param  name        Name of the argument
 *  @param  classname   If a specific class is required, the class type
 *  @param  null        Are NULL values allowed in stead of an instance?
 *  @param  ref         Is this a pass-by-reference argument?
 */
Argument::Argument(const std::string &name, const std::string &classname, bool null, bool ref)
{
    _refcount = new int[1];
    _info = new ArgInfo(name, classname, null, ref);
}

/**
 *  Constructor if the argument can be anything
 *  Note that only arrayType and callableType are supported type-hints
 *  @param  name        Name of the argument
 *  @param  type        Type hint (arrayType or callableType)
 *  @param  ref         Is this a pass-by-reference argument?
 */
Argument::Argument(const std::string &name, Type type, bool ref)
{
    _refcount = new int[1];
    _info = new ArgInfo(name, type, ref);
}

/**
 *  Constructor if the argument can be anything
 *  @param  name        Name of the argument
 *  @param  ref         Is this a pass-by-reference argument?
 */
Argument::Argument(const std::string &name, bool ref)
{
    _refcount = new int[1];
    _info = new ArgInfo(name, ref);
}

/**
 *  Clean up the object
 */
void Argument::cleanup()
{
    // one reference less
    (*_refcount)--;
    
    // leap out if still in use
    if (*_refcount > 0) return;
    
    // release memory
    delete _refcount;
    delete _info;
}

/**
 *  End of namespace
 */
}
