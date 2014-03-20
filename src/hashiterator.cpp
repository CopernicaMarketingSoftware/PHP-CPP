/**
 *  HashIterator.cpp
 *
 *  Implementation of the value iterator
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
 *  @param  hashtable       The hashtable to iterate over
 *  @param  first           Should it start at the first position?
 */
HashIterator::HashIterator(HashTable *hashtable, bool first) : _table(hashtable)
{
    // reset the hash pointer to the internal position
    if (hashtable && first) 
    {
        // move to first position
        zend_hash_internal_pointer_reset_ex(_table, &_position);
        
        // read current data
        if (read()) return;
        
        // data was private, move on
        increment();
    }
    else
    {
        // start with invalid data
        invalidate();
    }
}

/**
 *  Increment position
 *  @return bool
 */
bool HashIterator::increment()
{
    // leap out if already on an invalid pos (behind the last pos)
    if (!_position) return false;
    
    // move the iterator forward
    if (zend_hash_move_forward_ex(_table, &_position) == SUCCESS)
    {
        // read current key and value
        if (read()) return true;
        
        // data was private or invalid, move further
        return increment();
    }
    else
    {
        // invalidate current position
        return invalidate();
    }
}

/**
 *  Decrement position
 *  @return bool
 */
bool HashIterator::decrement()
{
    // leap out if we're not even iterating over a hash table
    if (!_table) return false;
    
    // if position is invalid, it is one position behind the last position
    if (!_position)
    {
        // move to last position
        zend_hash_internal_pointer_end_ex(_table, &_position);
    }
    else if (zend_hash_move_backwards_ex(_table, &_position) == FAILURE)
    {
        // invalidate current position
        return invalidate();
    }

    // read current key and value
    if (read()) return true;
    
    // data was private, move on
    return decrement();
}

/**
 *  Read current key and value
 *  @return bool        true if the object is in a valid position, false otherwise
 */
bool HashIterator::read()
{
    // zval to read the current key in
    Value key;

#if PHP_VERSION_ID >= 50500

    // read in the current key
    zend_hash_get_current_key_zval_ex(_table, key._val, &_position);
    
    // if the key is set to NULL, it means that the object is not at a valid position
    if (key.isNull()) return invalidate();
    
#else

    // php 5.3 and php 5.4 need a different implementation because the function
    // zend_hash_get_current_key_zval_ex is missing in php 5.3, declare variables
    // we need for storing the key in
    char *string_key;
    unsigned int str_len;
    unsigned long num_key;
    
    // get the current key
    int type = zend_hash_get_current_key_ex(_table, &string_key, &str_len, &num_key, 0, &_position);
    
    // if key is not found, the iterator is at an invalid position
    if (type == HASH_KEY_NON_EXISTANT) return invalidate();

    // numeric keys are the easiest ones
    if (type == HASH_KEY_IS_LONG) key = (int64_t)num_key;
    else key = string_key;

#endif

    // iterator is at a valid position, go fetch the data
    // this is the variable we need for fetching the data
    zval **value;
            
    // retrieve data
    zend_hash_get_current_data_ex(_table, (void **) &value, &_position);
    
    // we can now update the current data
    _current = std::make_pair<Value,Value>(std::move(key), *value);

    // if the key is private (it starts with a null character) we should return
    // false to report that the object is not in a completely valid state
    return !_current.first.isString() || _current.first.rawValue()[0];
}

/**
 *  Invalidate the iterator
 *  @return bool        always false
 */
bool HashIterator::invalidate()
{
    // forget current position
    _position = nullptr;
    
    // make the data a pair of null ptrs
    _current = std::make_pair<Value,Value>(nullptr,nullptr);
    
    // done
    return false;
}

/**
 *  End namespace
 */
}

