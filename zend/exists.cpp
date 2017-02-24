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
#include "string.h"

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
    // should we autoload the class?
    if (autoload)
    {
        // retrieve class entry
        auto *ce = zend_lookup_class(String{ classname, len });

        // no auto-load
        if (!ce) return false;

        // the found "class" could also be an interface or trait, which we do no want
        return (ce->ce_flags & (ZEND_ACC_INTERFACE | (ZEND_ACC_TRAIT - ZEND_ACC_EXPLICIT_ABSTRACT_CLASS))) == 0;
    }
    else
    {
        // starting slashes can be ignored
        if (len > 0 && classname[0] == '\\') { classname++; len--; }

        // create the string wrapper
        String string{ classname, len };

        // copy the name to lowercase, but ignore the starting slash (if there is one)
        zend_str_tolower(string.data(), string.size());

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
