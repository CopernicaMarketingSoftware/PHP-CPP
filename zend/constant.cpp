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
 *  Add the constant to a class
 * 
 *  You normally do not have to call this method yourself. You can simply
 *  do one of the following method calls to create class constants:
 * 
 *      myclass.property("MY_CONSTANT", "value", Php::Const);
 *      myclass.constant("MY_CONSTANT", "value");
 *      myclass.add(Php::Constant("MY_CONSTANT", "value"));
 * 
 *  All of the calls have the same result, it is up to you to decide which
 *  one suits you most. If you use the last one - using a Php::Constant
 *  class - the PHP-CPP library will call this "addTo()" method internally
 *  to forward the call to one of the other methods.
 * 
 *  @param  clss        Class to which the constant is added
 *  @internal
 */
void Constant::addTo(ClassBase &clss) const
{
    // pass on to the implementation
    _impl->addTo(clss);
}

/**
 *  End of namespace
 */
}

