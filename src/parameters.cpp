/**
 *  Parameters.cpp
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
 *  Parameters
 *  @param  this_ptr    Pointer to the object
 *  @param  argc        Number of arguments
 *  @param  tsrm_ls
 */
Parameters::Parameters(zval *this_ptr, int argc TSRMLS_DC)
{
    // reserve plenty of space
    reserve(argc);
    
    // loop through the arguments
    for (int i=0; i<argc; i++)
    {
        // get the argument
        zval **arg = (zval **) (zend_vm_stack_top(TSRMLS_C) - 1 - (argc-i));
        
        // append value
        push_back(Value(*arg));
    }
    
    // skip if there is no this_ptr
    if (!this_ptr) return;

    // get the mixed object
    MixedObject *obj = (MixedObject *)zend_object_store_get_object(this_ptr TSRMLS_CC);
    
    // store the CPP object
    _object = obj->cpp;
}

/**
 *  End of namespace
 */
}

