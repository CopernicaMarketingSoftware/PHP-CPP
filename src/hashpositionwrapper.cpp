/**
 *  hashpositionwrapper.cpp
 *
 *  HashPositionWrapper - designed for natural iteration objects and arrays of type Value. 
 *  it does not use STL containers and carries minimal overhead.
 *
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {


/**
 *  Constructor HashPositionWrapper
 *  @param  arr HashTable
 */
 HashPositionWrapper::HashPositionWrapper(_hashtable *arr) {
    ht = arr;
    pos = arr->pInternalPointer;
 }

/**
 *  reset iterator to beginning of the hash table
 */
void HashPositionWrapper::toBegin()
{
    zend_hash_internal_pointer_reset_ex(ht, &pos);
}

/**
 *  reset iterator to end of the hash table
 */
void HashPositionWrapper::toEnd()
{
    zend_hash_internal_pointer_end_ex(ht, &pos);
}

/**
 *  retrieve data value
 */
Value HashPositionWrapper::value() const {
    //zval **value;
    //zend_hash_get_current_data_ex(ht, (void **) &value, &pos);
    //return Value(*value);

    //zval_add_ref((zval **)pos->pData);
    return Value(*((zval **)pos->pData));
}

/**
 *  return string key
 */
//const char* HashPositionWrapper::key() const {
std::string HashPositionWrapper::key() const {
    //return (0 == pos->nKeyLength) ? "" : pos->arKey;
    return (0 == pos->nKeyLength) ? std::to_string(pos->h) : std::string(pos->arKey, pos->nKeyLength);
}

/**
 *  return integer key (index)
 */
unsigned long HashPositionWrapper::ind() const {
    return (0 == pos->nKeyLength) ? pos->h : 0;
}

/**
 *  key type is string?
 */
bool HashPositionWrapper::isstr() const {
    return (0 != pos->nKeyLength);
}

/**
 *  Check the permissions of key.
 *  It is used when the iteration of object.
 */
bool HashPositionWrapper::keyAccessible() const
{
    // Objects contain only string keys
    // But the fields with private/protected access key name missing
    // This hack and it might break in future versions of PHP.
    // @todo It would be good to redo it using regular Zend functions.
    return ( '\0' != *(pos->arKey) );
}

/**
 *  next iteration
 */
void HashPositionWrapper::next() {
    zend_hash_move_forward_ex(ht, &pos);
}

/**
 *  previous iteration
 */
void HashPositionWrapper::prev() {
    zend_hash_move_backwards_ex(ht, &pos);
}

/**
 *  compare operator
 */
bool HashPositionWrapper::operator==(const HashPositionWrapper& rhs) const {
    // If one of items is empty
    /*
    if( isEmpty() &&  rhs.isEmpty() ) return true;
    if( isEmpty() && !rhs.isEmpty() ) return false;
    if(!isEmpty() &&  rhs.isEmpty() ) return false;
    */

    // If both are not empty
    return (pos->h == rhs.pos->h && pos->nKeyLength == rhs.pos->nKeyLength && pos->arKey == rhs.pos->arKey );
}

/**
 *  End of namespace
 */
}
