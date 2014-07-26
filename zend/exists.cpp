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

    // we're going to load a class-entry
    zend_class_entry **ce;

    // should we autoload the class?
    if (autoload) 
    {
        // no auto-load
        if (SUCCESS != zend_lookup_class(classname, len, &ce TSRMLS_CC)) return false;

        // the found "class" could also be an interface or trait, which we do no want
        return ((*ce)->ce_flags & (ZEND_ACC_INTERFACE | (ZEND_ACC_TRAIT - ZEND_ACC_EXPLICIT_ABSTRACT_CLASS))) == 0;
    }
    else
    {
        // starting slashes can be ignored
        if (len > 0 && classname[0] == '\\') { classname++; len--; }
        
        // all classes are in lowercase in the hash, so we make 
        // a temporary buffer for storing the lowercase class name
        // (is this smart? memory allocation is expensive!)
        std::unique_ptr<char[]> lc_name(new char[len + 1]);
        
        // copy the name to lowercase, but ignore the starting slash (if there is one)
        zend_str_tolower_copy(lc_name.get(), classname, len);

        // see if there is a class with this name
        if (SUCCESS != zend_hash_find(EG(class_table), lc_name.get(), len + 1, (void **) &ce)) return false;
        
        // the found "class" could also be an interface or trait, which we do no want
        return !(((*ce)->ce_flags & (ZEND_ACC_INTERFACE | ZEND_ACC_TRAIT)) > ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);
    }
}

/**
 *  End of namespace
 */
}
