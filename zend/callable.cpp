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
void Callable::invoke(INTERNAL_FUNCTION_PARAMETERS)
{
    // find the function name
    const char *name = get_active_function_name(TSRMLS_C);
    
    // uncover the hidden pointer inside the function name
    Callable *callable = HiddenPointer<Callable>(name);

    // check if sufficient parameters were passed (for some reason this check
    // is not done by Zend, so we do it here ourselves)
    if (ZEND_NUM_ARGS() < callable->_required)
    {
        // PHP itself only generates a warning when this happens, so we do the same too
        Php::warning << name << "() expects at least " << callable->_required << " parameters, " << ZEND_NUM_ARGS() << " given" << std::flush;

        // and we return null
        RETURN_NULL();
    }
    else
    {
        // construct parameters
        ParametersImpl params(this_ptr, ZEND_NUM_ARGS() TSRMLS_CC);

        // the function could throw an exception
        try
        {
            // get the result
            Value result(callable->invoke(params));

            // we're ready if the return value is not even used
            if (!return_value_used) return;

            // @todo php 5.6 has a RETVAL_ZVAL_FAST macro that can be used instead (and is faster)

            // return a full copy of the zval, and do not destruct it
            RETVAL_ZVAL(result._val, 1, 0);
        }
        catch (Exception &exception)
        {
            // process the exception
            process(exception TSRMLS_CC);
        }
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
    entry->handler = &Callable::invoke;
    entry->arg_info = _argv;
    entry->num_args = _argc;
    entry->flags = flags;

    // we should fill the first argument as well
    initialize((zend_arg_info *)entry->arg_info, classname);
}

/**
 *  Fill a function entry
 *  @param  info        Info to be filled
 *  @param  classname   Optional classname
 */
void Callable::initialize(zend_arg_info *info, const char *classname) const
{
#if PHP_VERSION_ID >= 50400
    // up until php 5.3, the first info object is filled with alternative information,
    // later it is casted to a zend_internal_function object
    auto *finfo = (zend_internal_function_info *)info;
    
    // fill in all the members, note that return reference is false by default,
    // because we do not support returning references in PHP-CPP, although Zend
    // engine allows it. Inside the name we hide a pointer to the current object
    finfo->_name = _ptr;
    finfo->_name_len = ::strlen(_ptr);
    finfo->_class_name = classname;

    // number of required arguments, and the expected return type
    finfo->required_num_args = _required;
    finfo->_type_hint = (unsigned char)_return;

    // we do not support return-by-reference
    finfo->return_reference = false;
 
# if PHP_VERSION_ID >= 50600
    // since php 5.6 there are _allow_null and _is_variadic properties. It's
    // not exactly clear what they do (@todo find this out) so for now we set
    // them to false
    finfo->_allow_null = false;
    finfo->_is_variadic = false;

# else
    // passing by reference is not used (only for php 5.4 and php 5.5)
    finfo->pass_rest_by_reference = false;
# endif

#else
    // php 5.3 code
    info->name = nullptr;
    info->name_len = 0;
    info->class_name = nullptr;
    info->class_name_len = 0;
    info->array_type_hint = false;
    info->allow_null = false;
    info->pass_by_reference = false;
    info->return_reference = false;
    info->required_num_args = _required;
#endif
}

/**
 *  End of namespace
 */
}


