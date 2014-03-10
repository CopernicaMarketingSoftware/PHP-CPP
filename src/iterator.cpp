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
 *  Iterator destructor method
 *  @param  iter
 */
void Iterator::destructor(zend_object_iterator *iter)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;
    
    // delete the iterator
    delete iterator;
    
    // free memory for the meta object
    efree(iter);
}

/**
 *  Iterator valid function
 *  Returns FAILURE or SUCCESS
 *  @param  iter
 *  @return int
 */
int Iterator::valid(zend_object_iterator *iter)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;
    
    // check if valid
    return iterator->valid() ? SUCCESS : FAILURE;
}

/**
 *  Fetch the current item
 *  @param  iter
 *  @param  data
 */
void Iterator::current(zend_object_iterator *iter, zval ***data)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;

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
 */
void Iterator::key(zend_object_iterator *iter, zval *key)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;
    
    // retrieve the key
    Value retval(iterator->key());

    // detach the underlying zval
    zval *zval = retval.detach();
    
    // copy it to the key
    ZVAL_ZVAL(key, zval, 1, 1);
}

/**
 *  Function to retrieve the current key, php 5.3 style
 *  @param  iter
 *  @param  str_key
 *  @param  str_key_len
 *  @param  int_key
 *  @return HASH_KEY_IS_STRING or HASH_KEY_IS_LONG
 */
int Iterator::key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;

    // retrieve the key
    Value retval(iterator->key());
    
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
 */
void Iterator::next(zend_object_iterator *iter)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;

    // call the next method
    iterator->next();
}

/**
 *  Rewind the iterator back to the start
 *  @param  iter
 */
void Iterator::rewind(zend_object_iterator *iter)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;
    
    // call the rewind method
    iterator->rewind();
}

/**
 *  Get access to all iterator functions
 *  @return zend_object_iterator_funcs
 */
zend_object_iterator_funcs *Iterator::functions()
{
    // static variable with all functions
    static zend_object_iterator_funcs funcs;
    
    // static variable that knows if the funcs are already initialized
    static bool initialized = false;
    
    // no need to set anything if already initialized
    if (initialized) return &funcs;
    
    // set the members
    funcs.dtor = &Iterator::destructor;
    funcs.valid = &Iterator::valid;
    funcs.get_current_data = &Iterator::current;
    funcs.get_current_key = &Iterator::key;
    funcs.move_forward = &Iterator::next;
    funcs.rewind = &Iterator::rewind;
    
    // invalidate is not yet supported
    funcs.invalidate_current = nullptr;
    
    // remember that functions are initialized
    initialized = true;
    
    // done
    return &funcs;
}

/**
 *  Internal method that returns the implementation object
 *  @return zend_object_iterator
 */
struct _zend_object_iterator *Iterator::implementation()
{
    // create an iterator
    zend_object_iterator *iterator = (zend_object_iterator *)emalloc(sizeof(zend_object_iterator));
    
    // initialize all properties
    iterator->data = this;
    iterator->index = 0;
    iterator->funcs = functions();

    // done
    return iterator;
}

/**
 *  End namespace
 */
}

