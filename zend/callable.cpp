/**
 *  Function.cpp
 *
 *  Implementation for the function class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 - 2019 Copernica BV
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
 *  @return integer
 */
void Callable::invoke(INTERNAL_FUNCTION_PARAMETERS)
{
    uint32_t argc       = EX(func)->common.num_args;
    zend_arg_info* info = EX(func)->common.arg_info;

#if PHP_VERSION_ID < 70200
    // Sanity check
    assert(info[argc].class_name != nullptr && info[argc].name == nullptr);

    // the callable we are retrieving
    Callable *callable = reinterpret_cast<Callable*>(info[argc].class_name);
#else
    // Sanity check
    assert(info[argc].type != 0 && info[argc].name == nullptr);

    // the callable we are retrieving
    Callable *callable = reinterpret_cast<Callable*>(info[argc].type);
#endif

    // check if sufficient parameters were passed (for some reason this check
    // is not done by Zend, so we do it here ourselves)
    if (ZEND_NUM_ARGS() < callable->_required)
    {
        // PHP itself only generates a warning when this happens, so we do the same too
        Php::warning << get_active_function_name() << "() expects at least " << callable->_required << " parameter(s), " << ZEND_NUM_ARGS() << " given" << std::flush;

        // and we return null
        RETURN_NULL();
    }
    else
    {
        // construct parameters
        ParametersImpl params(getThis(), ZEND_NUM_ARGS());

        // the function could throw an exception
        try
        {
            // get the result
            Value result(callable->invoke(params));
            
            // return a full copy of the zval, and do not destruct it
            RETVAL_ZVAL(result._val, 1, 0);
        }
        catch (Throwable &throwable)
        {
            // an exception was not caught by the extension, let it bubble up
            throwable.rethrow();
        }
    }
}

/**
 *  Fill a function entry
 *
 *  This method is called when the extension is registering itself, when the
 *  function or method introduces himself
 *
 *  @param  entry       Entry to be filled
 *  @param  classname   Optional class name
 *  @param  flags       Is this a public property?
 */
void Callable::initialize(zend_function_entry *entry, const char *classname, int flags) const
{
    // do we already have a proper callback function?
    if (_callback)
    {
        // that's nice, we simply install the callback
        // no need to store any data in a lookup map
        entry->handler = _callback;
    }
    else
    {
        // install ourselves in the extra argument
#if PHP_VERSION_ID < 70200
        _argv[_argc + 1].class_name = reinterpret_cast<const char*>(this);
#else
        // @todo    this is broken. the zend engine, from 7.2 onwards copies over
        //          the struct and slices of the last element, because the num_args
        //          is incorrect in their view. another place to put this may be
        //          hiding it behind the fname
        _argv[_argc + 1].type = reinterpret_cast<zend_type>(this);
#endif

        // we use our own invoke method, which does a lookup
        // in the map we just installed ourselves in
        entry->handler = &Callable::invoke;
    }

    // fill the members of the entity, and hide a pointer to the current object in the name
    entry->fname = _name.data();
    entry->arg_info = _argv.get();
    entry->num_args = _argc;
    entry->flags = flags;

    // we should fill the first argument as well
    initialize((zend_internal_function_info*)_argv.get(), classname);
}

/**
 *  Fill a function entry
 *  @param  info        Info to be filled
 *  @param  classname   Optional classname
 */
void Callable::initialize(zend_internal_function_info *info, const char *classname) const
{
    // initialize all common elements
    info->required_num_args = _required;
    info->return_reference = false;
    info->_is_variadic = false;

    // the structure has been slightly altered since php7.2
#if PHP_VERSION_ID < 70200
    // store the classname
    info->class_name = classname;

    // number of required arguments, and the expected return type
    info->type_hint = (unsigned char)_return;

    // since php 5.6 there are _allow_null and _is_variadic properties. It's
    // not exactly clear what they do (@todo find this out) so for now we set
    // them to false
    info->allow_null = false;
#else
    // the properties that are available on php 7.2 and higher
    info->required_num_args = _required;
    info->return_reference = false;
    info->_is_variadic = false;
    info->type = ZEND_TYPE_ENCODE((int)_return, true);
#endif
}

/**
 *  End of namespace
 */
}


