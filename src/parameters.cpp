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
Parameters::Parameters(zval *this_ptr, int argc TSRMLS_DC) : _this(this_ptr)
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
}

/**
 *  The the object that is called
 *  @return Base
 */
Base *Parameters::object()
{
    // do we have a this pointer in the first place? The member is not set
    // when static methods are being called, or when a regular function is
    // called in a static context
    if (!_this) return nullptr;
    
    // get the mixed object
    MixedObject *obj = (MixedObject *)zend_object_store_get_object(_this TSRMLS_CC);
    
    // return the CPP object
    return obj->cpp;
}

/**
 *  End of namespace
 */
}

