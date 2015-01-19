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
Constant::Constant(const char *name, std::nullptr_t value) : 
    _impl(new ConstantImpl(name, value)) {}
    
/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, bool value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, int32_t value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, int64_t value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, double value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, const char *value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 *  @param  size            Value size
 */
Constant::Constant(const char *name, const char *value, size_t size) : 
    _impl(new ConstantImpl(name, value, size)) {}

/**
 *  Constructor
 *  @param  name            Constant name
 *  @param  value           Constant value
 */
Constant::Constant(const char *name, const std::string &value) : 
    _impl(new ConstantImpl(name, value)) {}

/**
 *  End of namespace
 */
}

