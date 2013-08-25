/**
 *  Callable.cpp
 *
 *  Implementation for the Callable class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace PhpCpp {

/**
 *  Function that is called by the Zend engine every time that a function gets called
 *  @param  mixed
 */
void invoke_callable(INTERNAL_FUNCTION_PARAMETERS)
{
    std::cout << "invoke_callable" << std::endl;
    
}

/**
 *  Fill a function entry
 *  @param  entry
 */
void Callable::fill(zend_function_entry *entry)
{
    // fill the members of the entity
    entry->fname = _function.c_str();
    entry->handler = invoke_callable;
    entry->arg_info = _argv;
    entry->num_args = _argc;
    entry->flags = _flags;
}

/**
 *  Another attempt to fill internal function info
 *  @param  entry
 */
void Callable::fill(zend_internal_function_info *info)
{
    // fill in all the members, not that the returning by reference is not used
    info->_name = _function.c_str();
    info->_name_len = _function.size();
    info->_class_name = _classname.size() ? _classname.c_str() : NULL;
    info->required_num_args = _required;
    info->_type_hint = _type;
    info->return_reference = false;
    info->pass_rest_by_reference = false;
}

/**
 *  Process the arguments
 *  @param  arguments
 */
void Callable::process(const std::initializer_list<Argument> &arguments)
{
    // store counters
    _argc = arguments.size();
    _required = arguments.size();
    
    // allocate memory for the arguments, with one extra record to hold information
    _argv = new zend_arg_info[_argc + 1];
    
    // fill the info
    fill((zend_internal_function_info *)_argv);
    
    // iteration counter
    int i = 0;
    
    // loop through the arguments
    for (auto it = begin(arguments); it != arguments.end(); it++)
    {
        // fill the argument structure
        it->internal()->fill(&_argv[++i]);
    }
}

/**
 *  End of namespace
 */
}

