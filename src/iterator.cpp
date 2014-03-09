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
    std::cout << "destruct iterator" << std::endl;
    
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
    std::cout << "Iterator::valid" << std::endl;
    
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
    std::cout << "get current value " << std::endl;
    
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;

    // retrieve the value
    Value value(iterator->current());

    std::cout << "detach value " << value << std::endl;

    zval *val = value.detach();
    
    // copy the value
    *data = &val;
}

/**
 *  Fetch the key for the current element (optional, may be NULL). The key
 *  should be written into the provided zval* using the ZVAL_* macros. If
 *  this handler is not provided auto-incrementing integer keys will be
 *  used.
 *  @param  iter
 *  @param  retval
 */
void Iterator::key(zend_object_iterator *iter, zval *retval)
{
    // get the actual iterator
    Iterator *iterator = (Iterator *)iter->data;
    
    // wrap data into a result object
//    Value result(data);
    
//    ZVAL_LONG(data, 123);
    
//    return;
//    std::cout << "retrieve key " << result.refcount() << std::endl;
    
    // retrieve the key as key
    Value keyValue = iterator->key();

    std::cout << "got key " << keyValue << " " << keyValue.refcount() << std::endl;

//    zval *zval = key.detach();


    std::cout << "ret key " << retval << " " << Z_REFCOUNT_P(retval) << std::endl;
    
    ZVAL_LONG(retval, rand());
    
//    ZVAL_ZVAL(data, zval, 1, 1);

    return;

    // copy the key into the other zval, but we use a string or numeric for
    // this operation, because we have looked at the implementation of Value
    // and assigning a full value to the result variable will cause the zval
    // to be destructed and re-allocated (which we do not need)
//    if (key.isString()) ZVAL_STRING(data, key.stringValue();
//    else ZVAL_LONG(data, key.numericValue());
//    
//    std::cout << "key is copied" << std::endl;
//
//    // detach from result
//    result.detach();
//    
    std::cout << "detached" << std::endl;
}

/**
 *  Step forwards to the next element
 *  @param  iter
 */
void Iterator::next(zend_object_iterator *iter)
{
    std::cout << "Iterator::next" << std::endl;
    
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
    std::cout << "Iterator::rewind" << std::endl;
    
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

