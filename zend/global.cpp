/**
 *  Global.cpp
 *
 *  Implementation for the global variable
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace Php {

/**
 *  Function that is called when the value is updated
 *  @return Value
 */
Global &Global::update()
{
    // skip if the variable already exists
    if (_exists) return *this;
    
    // we need the TSRMLS variable
    TSRMLS_FETCH();

    // add the variable to the globals
    zend_hash_add(EG(active_symbol_table), _name.c_str(), _name.size()+1, &_val, sizeof(zval*), NULL);

    // add one extra reference because the variable now is a global var too
    Z_ADDREF_P(_val);

    // remember that the variable now exists
    _exists = true;
    
    // done
    return *this;
}

/**
 *  End of namespace
 */
}

