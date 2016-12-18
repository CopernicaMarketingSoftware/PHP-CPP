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
 *  Constructor
 *  @param  zval            The object that is being iterated
 *  @param  iterator        The iterator that is implemented by the extension
 */
IteratorImpl::IteratorImpl(zval *object, Iterator *iterator) : _userspace(iterator)
{
    // initialize the iterator
    zend_iterator_init(&_iterator);

    // copy the object to the iterator, and set the callbacks
	ZVAL_COPY(&_iterator.data, object);
	_iterator.funcs = functions();
}

/**
 *  Destructor
 */
IteratorImpl::~IteratorImpl()
{
    // invalidate current
    invalidate();
    
    // one reference less to the original object
	zval_ptr_dtor(&_iterator.data);
}

/**
 *  Helper method to get access to ourselves
 *  @param  iter
 *  @return IteratorImpl
 */
IteratorImpl *IteratorImpl::self(zend_object_iterator *iter)
{
    // cast to the the other variable
    return (IteratorImpl *)iter;
}

/**
 *  Iterator destructor method
 *  @param  iter
 */
void IteratorImpl::destructor(zend_object_iterator *iter)
{
    // we are not going to deallocate the memory, because the php engine
    // seems to do that automagically nowadays, we just call the destructor explicitly
    self(iter)->~IteratorImpl();
}

/**
 *  Iterator valid function
 *  Returns FAILURE or SUCCESS
 *  @param  iter
 *  @return int
 */
int IteratorImpl::valid(zend_object_iterator *iter)
{
    // check if valid
    return self(iter)->valid() ? SUCCESS : FAILURE;
}

/**
 *  Fetch the current item
 *
 *  @param  iter    The iterator to retrieve the value from
 *  @return The current value of the iterator
 */
zval *IteratorImpl::current(zend_object_iterator *iter)
{
    // get the actual iterator
    auto *iterator = self(iter);

    // retrieve the value (and store it in a member so that it is not
    // destructed when the function returns)
    auto &value = iterator->current();

    // return the internal zval
    return value._val;
}

/**
 *  Fetch the key for the current element (optional, may be NULL). The key
 *  should be written into the provided zval* using the ZVAL_* macros. If
 *  this handler is not provided auto-incrementing integer keys will be
 *  used.
 *  @param  iter
 *  @param  key
 */
void IteratorImpl::key(zend_object_iterator *iter, zval *key)
{
    // retrieve the key
    Value retval(self(iter)->key());

    // detach the underlying zval
    Zval val = retval.detach(true);

    // copy it to the key
    ZVAL_ZVAL(key, val, 1, 1);
}

/**
 *  Step forwards to the next element
 *  @param  iter
 */
void IteratorImpl::next(zend_object_iterator *iter)
{
    // call the next method
    self(iter)->next();
}

/**
 *  Rewind the iterator back to the start
 *  @param  iter
 */
void IteratorImpl::rewind(zend_object_iterator *iter)
{
    // call the rewind method
    self(iter)->rewind();
}

/**
 *  Invalidate the current value of the object
 *  @param  iter
 */
void IteratorImpl::invalidate(zend_object_iterator *iter)
{
    // call the rewind method
    self(iter)->invalidate();
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
    funcs.invalidate_current = &IteratorImpl::invalidate;

    // remember that functions are initialized
    initialized = true;

    // done
    return &funcs;
}

/**
 *  End namespace
 */
}

