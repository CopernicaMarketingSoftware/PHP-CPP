/**
 *  ClassInfo.cpp
 *
 *  Implementation for the class info
 *
 *  @documentation private
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
void invoke_method(INTERNAL_FUNCTION_PARAMETERS)
{
    std::cout << "invoke method" << std::endl;
    
    return;
    
    // find the function name
    const char *name = get_active_function_name(TSRMLS_C);
    
    // uncover the hidden pointer inside the function name
    Function *function = HiddenPointer<Function>(name);

    // wrap the return value
    Value result(return_value, true);

    // construct parameters
    Parameters params(ZEND_NUM_ARGS());

	// get the result
	result = function->invoke(*PHPCPP_G(environment), params);
}

/**
 *  Helper struct to create an internal method
 */


/**
 *  Initialize the class
 *  @param  mixed       Optional threading ID 
 */
void _ClassInfo::initialize(TSRMLS_D)
{
    // the class entry
    zend_class_entry entry;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), NULL);

    // functions we need
    // @todo should not be static
    static InternalFunction constructor(invoke_method, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC);

    // we need a special constructor
    entry.__call = constructor;
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);
}

/**
 *  End of namespace
 */
}
 
