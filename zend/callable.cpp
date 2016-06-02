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
 *  Map function names to their implementation
 *
 *  This is braindead, there should be a way to get this information
 *  from the "This" zval in the execute_data, I just can't find it
 *  @todo   Find a better way for this
 *  @var    std::map<std::string, Callable*>
 */
static std::map<std::string, Callable*> callables;

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
    const char *classname = get_active_class_name(nullptr TSRMLS_C);

    // the callable we are retrieving
    Callable *callable = nullptr;

    // are we invoking a member function?
    if (classname && classname[0])
    {
        // construct the full name to search for
        auto fullname = std::string{ classname } + "::" + name;

        // and find the callable in the map
        callable = callables.find(fullname)->second;
    }
    else
    {
        // retrieve the callable from the map without mangling
        callable = callables.find(name)->second;
    }

    // check if sufficient parameters were passed (for some reason this check
    // is not done by Zend, so we do it here ourselves)
    if (ZEND_NUM_ARGS() < callable->_required)
    {
        // PHP itself only generates a warning when this happens, so we do the same too
        Php::warning << name << "() expects at least " << callable->_required << " parameter(s), " << ZEND_NUM_ARGS() << " given" << std::flush;

        // and we return null
        RETURN_NULL();
    }
    else
    {
        // construct parameters
        ParametersImpl params(getThis(), ZEND_NUM_ARGS() TSRMLS_CC);

        // the function could throw an exception
        try
        {
            // get the result
            Value result(callable->invoke(params));

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
        // if we have a classname we have to combine the two for a unique name
        // otherwise similar functions  - like __construct - will overwrite functions
        // declared before
        if (classname)
        {
            // build the unique name
            auto name = std::string{ classname } + "::" + _name;

            // add it to the map
            callables[name] = const_cast<Callable*>(this);
        }
        else
        {
            // no need to mangle the name
            callables[_name] = const_cast<Callable*>(this);
        }

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
    // store the classname
    info->class_name = classname;

    // number of required arguments, and the expected return type
    info->required_num_args = _required;
    info->type_hint = (unsigned char)_return;

    // we do not support return-by-reference
    info->return_reference = false;

    // since php 5.6 there are _allow_null and _is_variadic properties. It's
    // not exactly clear what they do (@todo find this out) so for now we set
    // them to false
    info->allow_null = false;
    info->_is_variadic = false;
}

/**
 *  End of namespace
 */
}


