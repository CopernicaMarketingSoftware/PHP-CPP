/**
 *  hashitemiterator.cpp
 *
 *
 *  HashItemIterator - allows iterate objects of a class that implements the PHP Iterator interface
 *
 *
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor HashItemIterator
 *  @param  arr HashTable
 */
HashItemIterator::HashItemIterator(zend_class_entry *ce, zval *pval)
{
    funcs = ce->iterator_funcs.funcs;
    iter = ce->get_iterator(ce, pval, 0);
}


/**
 *  retrieve data value
 */
Value HashItemIterator::value() const 
{
    // fetch the item data for the current element
    zval **pval;
    funcs->get_current_data(iter, &pval);
    return *pval;
}

/**
 *  return key
 */
Value HashItemIterator::key() const
{
    // fetch the key for the current element. 
    // The key should be written into the provided zval* using the ZVAL_* macros.
    // If this handler is not provided auto-incrementing integer keys will be used.
    zval zv;
    funcs->get_current_key(iter, &zv);
    Value retv(&zv);

    // @todo May be the following line can be done better?
    return retv.clone();
}

/**
 *  return integer key (index)
 */
unsigned long HashItemIterator::intKey() const
{
    zval zv;
    funcs->get_current_key(iter, &zv);
    return zv.value.lval;
}

/**
 *  return string key
 */
std::string HashItemIterator::strKey() const
{
    zval zv;
    funcs->get_current_key(iter, &zv);
    return  std::string(zv.value.str.val, zv.value.str.len);
}

/**
 *  key type is string?
 */
bool HashItemIterator::isstr() const
{
    zval zv;
    funcs->get_current_key(iter, &zv);

    return ( (Type)Z_TYPE(zv) == Type::String );   
}

/**
 *  is hashtable item is empty?
 */
bool HashItemIterator::isEmpty() const
{
    // check for end of iteration (FAILURE or SUCCESS if data is valid)
    return ( FAILURE == funcs->valid(iter) );
}

/**
 *  next iteration
 */
void HashItemIterator::next()
{
    // step forwards to next element
    funcs->move_forward(iter);
}

/**
 *  reset iterator to beginning of the hash table
 */
void HashItemIterator::reset()
{
    // rewind to start of data
    funcs->rewind(iter);
}

/**
 *  compare operator
 */
bool HashItemIterator::compare(const HashItem *rhs) const
{
    zval thisKey, thatKey;
    funcs->get_current_key(iter, &thisKey);
    ((HashItemIterator *)rhs)->funcs->get_current_key( ((HashItemIterator *)rhs)->iter, &thatKey);

    return (
                thisKey.type          == thatKey.type          && 
                thisKey.value.lval    == thatKey.value.lval    && 
                thisKey.value.str.len == thatKey.value.str.len &&
                thisKey.value.str.val == thatKey.value.str.val
           );
    return true;
}

HashItemIterator::~HashItemIterator()
{
    // release all resources associated with this iterator instance
    funcs->dtor(iter);
}

/**
 *  End of namespace
 */
}
