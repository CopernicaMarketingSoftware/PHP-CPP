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
 *  Constructor
 *  @param  name        Name of the argument
 *  @param  type        Argument type
 *  @param  required    Is this argument required?
 *  @param  byref       Is this a reference argument
 */
Argument::Argument(const char *name, Type type, bool required, bool byref)
{
    // construct object
    _info = new zend_arg_info;
    
    // fill members
    _info->name = name;
    _info->name_len = strlen(name);
#if PHP_VERSION_ID >= 50400    
    _info->type_hint = (unsigned char)(type == Type::Array || type == Type::Callable ? type : Type::Null);
#else
    _info->array_type_hint = type == Type::Array;
    _info->return_reference = false;
    _info->required_num_args = 0;   // @todo is this correct?
#endif
    _info->class_name = NULL;
    _info->class_name_len = 0;
    _info->allow_null = false;
    _info->pass_by_reference = byref;
    
    // store if required
    _required = required;
}

/**
 *  Constructor
 *  @param  name        Name of the argument
 *  @param  classname   Name of the class
 *  @param  nullable    Can it be null?
 *  @param  required    Is this argument required?
 *  @param  byref       Is this a reference argument?
 */
Argument::Argument(const char *name, const char *classname, bool nullable, bool required, bool byref)
{
    // construct object
    _info = new zend_arg_info;
    
    // fill members
    _info->name = name;
    _info->name_len = strlen(name);
#if PHP_VERSION_ID >= 50400    
    _info->type_hint = (unsigned char)Type::Object;
#endif
    _info->class_name = classname;
    _info->class_name_len = strlen(classname);
    _info->allow_null = nullable;
    _info->pass_by_reference = byref;
    
    // store if required
    _required = required;
}

/**
 *  Copy constructor
 *  @param  argument
 */
Argument::Argument(const Argument &argument)
{
    // construct object
    _info = new zend_arg_info;
    
    // fill members
    *_info = *argument._info;
    
    // store if required
    _required = argument._required;
}

/**
 *  Move constructor
 *  @param  argument
 */
Argument::Argument(Argument &&argument)
{
    // copy memory pointer
    _info = argument._info;
    
    // forget in other object
    argument._info = nullptr;
    
    // store if required
    _required = argument._required;
}

/**
 *  Destructor
 */
Argument::~Argument()
{
    if (_info) delete _info;
}

/**
 *  Fill an arg_info structure with data
 *  @param  info
 *  @internal
 */
void Argument::fill(struct _zend_arg_info *info) const
{
    // copy all data
    *info = *_info;
}

/**
 *  End of namespace
 */
}
