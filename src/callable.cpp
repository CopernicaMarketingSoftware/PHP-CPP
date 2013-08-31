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
 *  @param  ht      
 *  @param  return_value
 *  @param  return_value_ptr
 *  @param  this_ptr
 *  @param  return_value_used
 *  @param  tsrm_ls
 *  @return integer
 */
void invoke_callable(INTERNAL_FUNCTION_PARAMETERS)
{
    // find the function name
    const char *function = get_active_function_name(TSRMLS_C);
    
    // uncover the hidden pointer inside the function name
    Callable *callable = HiddenPointer<Callable>(function);

    // call the appropriate object
    callable->invoke(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

/**
 *  Fill a function entry
 * 
 *  This method is called when the extension is registering itself, when the 
 *  function or method introces himself
 * 
 *  @param  entry
 */
void Callable::fill(zend_function_entry *entry)
{
    // fill the members of the entity, and hide a pointer to the current object in the name
    entry->fname = HiddenPointer<Callable>(this, _name);
    entry->handler = invoke_callable;
    entry->arg_info = _argv;
    entry->num_args = _argc;
    entry->flags = _flags;
}

/**
 *  Another attempt to fill internal function info
 * 
 *  This method is called when the extension is registering itself, when the 
 *  function or method introces himself
 * 
 *  @param  entry
 */
void Callable::fill(zend_internal_function_info *info)
{
    // fill in all the members, note that return reference is false by default,
    // because we do want to return references, inside the name we hide a pointer
    // to the current object
    info->_name = HiddenPointer<Callable>(this, _name);
    info->_name_len = _name.size();
    info->_class_name = _classname.size() ? _classname.c_str() : NULL;
    info->required_num_args = _required;
    info->_type_hint = _type;
    info->return_reference = false;
    info->pass_rest_by_reference = false;
}

/**
 *  Process the arguments
 * 
 *  The arguments are called by the user of the PhpCpp library when he 
 * 
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

int do_test(int a, int b)
{
    std::cout << "do_test: " << a << " " << b << std::endl;
    
    return 77;
}

/**
 *  Invoke the method
 *  @param  ht      
 *  @param  return_value
 *  @param  return_value_ptr
 *  @param  this_ptr
 *  @param  return_value_used
 *  @param  tsrm_ls
 *  @return integer
 */
int Callable::invoke(INTERNAL_FUNCTION_PARAMETERS)
{
    std::cout << "args: " << ZEND_NUM_ARGS() << std::endl;
    std::cout << "required: " << _required << std::endl;
    std::cout << "argc: " << _argc << std::endl;
    
    // number of arguments should be sufficient // @todo show error message
//    if (ZEND_NUM_ARGS() < _required) return FAILURE;
    
    // and not be too much // @todo show error message
//    if (ZEND_NUM_ARGS() > _argc) return FAILURE;
    
    // number of arguments on the stack
    int arg_count = (int)(zend_uintptr_t) *(zend_vm_stack_top(TSRMLS_C) - 1);
    
//    // loop through the arguments
//    Arguments args(ZEND_NUM_ARGS());
//    
//    for (auto iter = args.begin(); iter != args.end(); iter++)
//    {
//        Value val = *iter;
//        
//        val = 1234;
//    }
//    
//    int result = do_test(args[1], args[2]);
//    
    Value ret(return_value, true);
    
    std::cout << "set property 1" << std::endl;
    
//    ret["b"] = "hallo";
    
    ret["x"]["c"]["d"] = "test 123";

    std::cout << "done setting properties" << std::endl;
    
    
//
//    // done
    return SUCCESS;
}




/**
 *  End of namespace
 */
}

