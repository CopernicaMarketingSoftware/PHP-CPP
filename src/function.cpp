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
void invoke_function(INTERNAL_FUNCTION_PARAMETERS)
{
    // find the function name
    const char *name = get_active_function_name(TSRMLS_C);
    
    // uncover the hidden pointer inside the function name
    Function *function = HiddenPointer<Function>(name);

    // wrap the return value
    Value result(return_value, true);

    // construct parameters
    Parameters params(this_ptr, ZEND_NUM_ARGS());

    // the function could throw an exception
    try
    {
        // get the result
        result = function->invoke(params);
    }
    catch (Php::OrigException &exception)
    {
        // we caught an exception that was original thrown by PHP code, and not 
        // processed by C++ code, this means that we're going to restore this 
        // exception so that it can be further handled by PHP
        exception.restore();
    }
    catch (Php::Exception &exception)
    {
        // an exception originally thrown by C++ should be passed on to PHP
        zend_throw_exception(zend_exception_get_default(), (char*)exception.message().c_str(), 0 TSRMLS_CC);
    }
}

/**
 *  Constructor
 *  @param  name    Name of the function
 *  @param  min     Min number of arguments
 *  @param  max     Max number of arguments
 */
Function::Function(const char *name, const std::initializer_list<Argument> &arguments) : _ptr(this, name)
{
    // construct vector for arguments
    _argc = arguments.size();
    _argv = new zend_arg_info[_argc+1];
    
    // counter
    int i=1;
    
    // loop through the arguments
    for (auto it = arguments.begin(); it != arguments.end(); it++)
    {
        // fill the arg info
        it->fill(&_argv[i++]);
    }
    
    // @todo find out number of required arguments
    _required = _argc;
}

/**
 *  Destructor
 */
Function::~Function()
{
    delete[] _argv;
}

/**
 *  Fill a function entry
 * 
 *  This method is called when the extension is registering itself, when the 
 *  function or method introces himself
 * 
 *  @param  entry       Entry to be filled
 *  @param  classname   Optional class name
 *  @param  pub         Is this a public property?
 */
void Function::fill(zend_function_entry *entry, const char *classname, bool pub) const
{
    // fill the members of the entity, and hide a pointer to the current object in the name
    entry->fname = _ptr;
    entry->handler = invoke_function;
    entry->arg_info = _argv;
    entry->num_args = _argc;

    // there are no flags like deprecated, private or protected
    entry->flags = classname ? (pub ? ZEND_ACC_PUBLIC : ZEND_ACC_PROTECTED) : 0;
    
    // we should fill the first argument as well
    fill((zend_internal_function_info *)entry->arg_info, classname);
}

/**
 *  Fill a function entry
 *  @param  info        Info to be filled
 *  @param  classname   Optional classname
 */
void Function::fill(zend_internal_function_info *info, const char *classname) const
{
    // fill in all the members, note that return reference is false by default,
    // because we do not support returning references in PHP-CPP, although Zend
    // engine allows it. Inside the name we hide a pointer to the current object
    info->_name = _ptr;
    info->_name_len = _ptr.length();
    info->_class_name = classname;
    
    // number of required arguments, and the expected return type
    info->required_num_args = _required;
    info->_type_hint = _type;
    
    // we do not support return-by-reference
    info->return_reference = false;
    
    // passing by reference is not used
    info->pass_rest_by_reference = false;
}

/**
 *  End of namespace
 */
}


