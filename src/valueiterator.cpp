/**
 *  valueiterator.cpp
 *
 *  ValueIterator - designed for natural iteration objects and arrays of type Value. 
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
 *  Constructor ValueIterator
 *  @param  arr HashTable
 */
ValueIterator::ValueIterator(_hashtable *arr, bool isArray): _arr(arr), _isArray(isArray) {
    // reset iterator to beginning of the hash table
    zend_hash_internal_pointer_reset(arr);

    // filling _pair
    setPair();
}

/**
 *  Retrieving data from `HashTable arr` and filling in the `ValuePair _pair`
 */
void ValueIterator::setPair() {
    // pointer that will be set to hash key or index
    char *key;
    unsigned long ind;

    // get current key
    int hash_key_type = zend_hash_get_current_key(_arr, &key, &ind, 0);

    // if data NO exist
    if(HASH_KEY_NON_EXISTENT == hash_key_type) {
        _pair.reset();
        return;
    }

    // required variable (array value)
    zval **value;

    // check Value type
    if (_isArray)
    {
        
        // retrieve data, and add to result
        zend_hash_get_current_data(_arr, (void **) &value);

        // check key type
        if (HASH_KEY_IS_STRING == hash_key_type) {
            _pair.isString = true;
            _pair._key     = key;
            _pair._ind     = 0;

        } else {
            _pair.isString = false;
            _pair._key     = "";
            _pair._ind     = ind;
        }
        _pair._value = value;
    }
    else
    {
        // inaccessible properties (privates) start with a null character
        if ('\0' == *key) {
            nextIteration();
            setPair();
            return;
        }

        // retrieve data, and add to result
        zend_hash_get_current_data(_arr, (void **) &value);

        // fill pair
        _pair.isString = true;
        _pair._key     = key;
        _pair._ind     = 0;
        _pair._value = value;
    }

}

/**
 *  next iteration
 */
void ValueIterator::nextIteration() {
    zend_hash_move_forward(_arr);
}
            

/**
 *  End of namespace
 */
}
