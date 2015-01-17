/**
 *  Constant.cpp
 * 
 *  Implementation file for the constant class
 *  
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, const Value &value) : 
    _impl(new ConstantImpl(name, value)) {}
    
/**
 *  End of namespace
 */
}

