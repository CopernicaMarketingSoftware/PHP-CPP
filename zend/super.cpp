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
 *  Convert object to a value
 *  @return Value
 */
Value Super::value()
{
    // we need the tsrm_ls pointer
    TSRMLS_FETCH();

    // call zend_is_auto_global to ensure that the just-in-time globals are loaded
    if (_name) { zend_is_auto_global(_name, ::strlen(_name) TSRMLS_CC); _name = nullptr; }
    
    // create a value object that wraps around the actual zval
    return Value(PG(http_globals)[_index]);
}

/**
 *  End namespace
 */
}

