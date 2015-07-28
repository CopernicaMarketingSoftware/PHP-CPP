/**
 *  IteratorImpl.cpp
 *
 *  Implementation file of the IteratorImpl class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Helper method to get access to ourselves
 *  @param  iter
 *  @return IteratorImpl
 */
static IteratorImpl *self(zend_object_iterator *iter)
{
    return (IteratorImpl *)iter->data;
}

/**
 *  Iterator destructor method
 *  @param  iter
 *  @param  tsrm_ls
 */
void IteratorImpl::destructor(zend_object_iterator *iter TSRMLS_DC)
{
    // delete the object
    delete self(iter);
}

/**
 *  Iterator valid function
 *  Returns FAILURE or SUCCESS
 *  @param  iter
 *  @param  tsrm_ls
 *  @return int
 */
int IteratorImpl::valid(zend_object_iterator *iter TSRMLS_DC)
{
    // check if valid
    return self(iter)->valid() ? SUCCESS : FAILURE;
}

/**
 *  Fetch the current item
 *  @param  iter
 *  @param  data
 *  @param  tsrm_ls
 */
void IteratorImpl::current(zend_object_iterator *iter, zval ***data TSRMLS_DC)
{
    // get the actual iterator
    IteratorImpl *iterator = self(iter);

    // retrieve the value (and store it in a member so that it is not
    // destructed when the function returns)
    iterator->_current = iterator->current();

    // copy the value
    *data = &iterator->_current._val;
}

/**
 *  Fetch the key for the current element (optional, may be NULL). The key
 *  should be written into the provided zval* using the ZVAL_* macros. If
 *  this handler is not provided auto-incrementing integer keys will be
 *  used.
 *  @param  iter
 *  @param  key
 *  @param  tsrm_ls
 */
void IteratorImpl::key(zend_object_iterator *iter, zval *key TSRMLS_DC)
{
    // retrieve the key
    Value retval(self(iter)->key());

    // detach the underlying zval
    zval *val = retval.detach(true);

    // copy it to the key
    ZVAL_ZVAL(key, val, 1, 1);
}

/**
 *  Function to retrieve the current key, php 5.3 style
 *  @param  iter
 *  @param  str_key
 *  @param  str_key_len
 *  @param  int_key
 *  @param  tsrm_ls
 *  @return HASH_KEY_IS_STRING or HASH_KEY_IS_LONG
 */
int IteratorImpl::key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key TSRMLS_DC)
{
    // retrieve the key
    Value retval(self(iter)->key());
    
    // is this a numeric string?
    if (retval.isString())
    {
        // copy the key and the from the value
        *str_key = estrndup(retval.rawValue(), retval.size());
        *str_key_len = retval.size() + 1;
        
        // done
        return HASH_KEY_IS_STRING;
    }
    else
    {
        // convert to a numeric
        *int_key = retval.numericValue();
        
        // done
        return HASH_KEY_IS_LONG;
    }
}

/**
 *  Step forwards to the next element
 *  @param  iter
 *  @param  tsrm_ls
 */
void IteratorImpl::next(zend_object_iterator *iter TSRMLS_DC)
{
    // call the next method
    self(iter)->next();
}

/**
 *  Rewind the iterator back to the start
 *  @param  iter
 *  @param  tsrm_ls
 */
void IteratorImpl::rewind(zend_object_iterator *iter TSRMLS_DC)
{
    // call the rewind method
    self(iter)->rewind();
}

/**
 *  Get access to all iterator functions
 *  @return zend_object_iterator_funcs
 */
zend_object_iterator_funcs *IteratorImpl::functions()
{
    // static variable with all functions
    static zend_object_iterator_funcs funcs;
    
    // static variable that knows if the funcs are already initialized
    static bool initialized = false;
    
    // no need to set anything if already initialized
    if (initialized) return &funcs;
    
    // set the members
    funcs.dtor = &IteratorImpl::destructor;
    funcs.valid = &IteratorImpl::valid;
    funcs.get_current_data = &IteratorImpl::current;
    funcs.get_current_key = &IteratorImpl::key;
    funcs.move_forward = &IteratorImpl::next;
    funcs.rewind = &IteratorImpl::rewind;
    
    // invalidate is not yet supported
    funcs.invalidate_current = nullptr;
    
    // remember that functions are initialized
    initialized = true;
    
    // done
    return &funcs;
}

/**
 *  End namespace
 */
}

