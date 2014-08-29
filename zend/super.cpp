/**
 *  Super.cpp
 *
 *  @copyright 2014 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  A number of super-globals are always accessible
 */
Super POST      (TRACK_VARS_POST,    "_POST");
Super GET       (TRACK_VARS_GET,     "_GET");
Super COOKIE    (TRACK_VARS_COOKIE,  "_COOKIE");
Super SERVER    (TRACK_VARS_SERVER,  "_SERVER");
Super ENV       (TRACK_VARS_ENV,     "_ENV");
Super FILES     (TRACK_VARS_FILES,   "_FILES");
Super REQUEST   (TRACK_VARS_REQUEST, "_REQUEST");

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Value
 */
Value Super::operator[](const std::string &key)
{
    // we need the tsrm_ls pointer
    TSRMLS_FETCH();
    
    // call zend_is_auto_global to ensure that the just-in-time globals are loaded
    if (_name) { zend_is_auto_global(_name, ::strlen(_name) TSRMLS_CC); _name = nullptr; }
    
    // create a value object that wraps around the actual zval
    Value value(PG(http_globals)[_index]);
    
    // pass on the call
    return value.get(key);
}

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Value
 */
Value Super::operator[](const char *key)
{
    // we need the tsrm_ls pointer
    TSRMLS_FETCH();

    // call zend_is_auto_global to ensure that the just-in-time globals are loaded
    if (_name) { zend_is_auto_global(_name, ::strlen(_name) TSRMLS_CC); _name = nullptr; }
    
    // create a value object that wraps around the actual zval
    Value value(PG(http_globals)[_index]);
    
    // pass on the call
    return value.get(key);
}

/**
 *  End namespace
 */
}

