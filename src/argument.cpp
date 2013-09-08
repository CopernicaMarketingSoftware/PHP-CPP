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
namespace Php {

/**
 *  Move constructor
 *  @param  argument
 */
Argument::Argument(Argument &&argument)
{
    // copy data
    _info = argument._info;
}

/**
 *  Change the name
 *  @param  name
 *  @return Argument
 */
Argument &Argument::name(const char *name)
{
    _info->name = name;
    _info->name_len = strlen(name);
    return *this;
}

/**
 *  Change the type
 *  @param  type
 *  @return Argument
 */
Argument &Argument::type(Type type)
{
    _info->type_hint = type;
    return *this;
}

/**
 *  Require the parameter to be a certain class
 *  @param  name        Name of the class
 *  @param  null        Are null values allowed?
 *  @return Argument
 */
Argument &Argument::object(const char *classname, bool null)
{
    _info->type_hint = objectType;
    _info->class_name = classname;
    _info->class_name_len = strlen(classname);
    _info->allow_null = null;
    return *this;
}

/**
 *  Is this a by-ref argument?
 *  @param  bool        Mark as by-ref variable
 *  @return Argument
 */
Argument &Argument::byref(bool value)
{
    _info->pass_by_reference = value;
    return *this;
}

/**
 *  End of namespace
 */
}
