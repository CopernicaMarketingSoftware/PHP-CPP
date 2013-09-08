/**
 *  Arguments.cpp
 *
 *  Implementation of the arguments class
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
 *  Constructor
 *  @param  min     The min number of arguments
 *  @param  max     The max number of arguments
 */
Arguments::Arguments(int min, int max)
{
    // copy arguments
    _min = min;
    _max = max;
    
    // max should be appropriate
    if (_max < _min) _max = _min;
    
    // allocate memory for the arguments, with one extra record to hold information
    _argv = new zend_arg_info[_max + 1];

    // initialize the arguments
    for (int i=1; i<_max+1; i++)
    {
        // initialize the argument
        _argv[i].name = NULL;
        _argv[i].name_len = 0;
        _argv[i].class_name = NULL;
        _argv[i].class_name_len = 0;
        _argv[i].type_hint = nullType;
        _argv[i].allow_null = false;
        _argv[i].pass_by_reference = false;
    }
}

/**
 *  Destructor
 */
Arguments::~Arguments()
{
    // deallocate arguments
    delete[] _argv;
}

/**
 *  Constructor
 *  @param  argc    Number of arguments
 *  @param  tsrm_ls
 */
/*

Arguments::Arguments(int argc TSRMLS_DC)
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
* 
*/
    
/**
 *  End of namespace
 */
}

