/**
 *  Exists.cpp
 *
 *  This file holds the implementation of all *_exists() functions,
 *  like class_exists(), et cetera
 *
 *  @author andot <https://github.com/andot>
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  On php 5.3 ZEND_ACC_TRAIT isn't defined, so we simply define it to 0
 *  so all operations with it are basically no-ops. Currently unconfirmed
 *  if this actually works correctly on php 5.3, but it at least compiles.
 */
#ifndef ZEND_ACC_TRAIT
#define ZEND_ACC_TRAIT 0
#endif

/**
 *  Open the PHP namespace
 */
namespace Php {

/**
 *  Check whether a class with a certain name exists
 *  @param  classname
 *  @param  len
 *  @param  autoload
 *  @return bool
 */
bool class_exists(const char *classname, size_t len, bool autoload)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // should we autoload the class?
    if (autoload)
    {
        // retrieve class entry
        auto *ce = zend_lookup_class(zend_string_init(classname, len, 1) TSRMLS_CC);

        // no auto-load
        if (!ce) return false;

        // the found "class" could also be an interface or trait, which we do no want
        return (ce->ce_flags & (ZEND_ACC_INTERFACE | (ZEND_ACC_TRAIT - ZEND_ACC_EXPLICIT_ABSTRACT_CLASS))) == 0;
    }
    else
    {
        // starting slashes can be ignored
        if (len > 0 && classname[0] == '\\') { classname++; len--; }

        // allocate a zend_string
        auto *string = zend_string_alloc(len, 1);

        // copy the name to lowercase, but ignore the starting slash (if there is one)
        zend_str_tolower_copy(ZSTR_VAL(string), classname, len);

        // see if there is a class with this name
        auto *val = zend_hash_find(EG(class_table), string);

        // check whether something was found
        if (val == nullptr) return false;

        // the "something" could also be an interface or trait, which we do no want
        return !(((Z_CE_P(val))->ce_flags & (ZEND_ACC_INTERFACE | ZEND_ACC_TRAIT)) > ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);
    }
}

/**
 *  End of namespace
 */
}
