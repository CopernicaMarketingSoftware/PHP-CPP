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
Super POST      (TRACK_VARS_POST);
Super GET       (TRACK_VARS_GET);
Super COOKIE    (TRACK_VARS_COOKIE);
Super SERVER    (TRACK_VARS_SERVER);
Super ENV       (TRACK_VARS_ENV);
Super FILES     (TRACK_VARS_FILES);
Super REQUEST   (TRACK_VARS_REQUEST);

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Value
 */
Value Super::operator[](const std::string &key) const
{
    // create a value object that wraps around the actual zval
    Value value(PG(http_globals)[_index]);
    
    // pass on the call
    return value[key];
}

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Value
 */
Value Super::operator[](const char *key) const
{
    // create a value object that wraps around the actual zval
    Value value(PG(http_globals)[_index]);
    
    // pass on the call
    return value[key];
}

/**
 *  End namespace
 */
}

