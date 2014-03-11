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
namespace Php {

/**
 *  Function that is called by the Zend engine every time that a function gets called
 *  @param  ht
 *  @param  return_value
 *  @param  return_value_ptr
 *  @param  this_ptr
 *  @param  return_value_used
 *  @param  tsrm_ls
 *  @return integer
 */
static void invoke_callable(INTERNAL_FUNCTION_PARAMETERS)
{
    // find the function name
    const char *name = get_active_function_name(TSRMLS_C);
    
    // uncover the hidden pointer inside the function name
    Callable *callable = HiddenPointer<Callable>(name);

    // wrap the return value
    Value result(return_value, true);

    // construct parameters
    Parameters params(this_ptr, ZEND_NUM_ARGS());

    // the function could throw an exception
    try
    {
        // get the result
        result = callable->invoke(params);
    }
    catch (Exception &exception)
    {
        // process the exception
        exception.process();
    }
}

/**
 *  Fill a function entry
 * 
 *  This method is called when the extension is registering itself, when the 
 *  function or method introces himself
 * 
 *  @param  entry       Entry to be filled
 *  @param  classname   Optional class name
 *  @param  flags       Is this a public property?
 */
void Callable::initialize(zend_function_entry *entry, const char *classname, int flags) const
{
    // fill the members of the entity, and hide a pointer to the current object in the name
    entry->fname = (const char *)_ptr;
    entry->handler = invoke_callable;
    entry->arg_info = _argv;
    entry->num_args = _argc;
    entry->flags = flags;

    // we should fill the first argument as well
#if PHP_VERSION_ID >= 50400
    initialize((zend_internal_function_info *)entry->arg_info, classname);
#endif
}

/**
 *  Fill a function entry
 *  @param  info        Info to be filled
 *  @param  classname   Optional classname
 */
#if PHP_VERSION_ID >= 50400
void Callable::initialize(zend_internal_function_info *info, const char *classname) const
{
    // fill in all the members, note that return reference is false by default,
    // because we do not support returning references in PHP-CPP, although Zend
    // engine allows it. Inside the name we hide a pointer to the current object
    info->_name = _ptr;
    info->_name_len = strlen(_ptr);
    info->_class_name = classname;

    // number of required arguments, and the expected return type
    info->required_num_args = _required;
    info->_type_hint = (unsigned char)_return;

    // we do not support return-by-reference
    info->return_reference = false;
 
    // passing by reference is not used
    info->pass_rest_by_reference = false;
}
#endif

/**
 *  End of namespace
 */
}


