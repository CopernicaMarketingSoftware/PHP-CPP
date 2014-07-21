/**
 *  HashIterator.h
 *
 *  This is an internal helper class that is used when iterating over a
 *  Php::Value object that holds a hash table (an array or an object that
 *  does not implement the Traversable interface - stl style.
 *
 *  Thus, when you do c++ things like "for (auto &iter : value)", internally
 *  a ValueIterator object is being used.
 *
 *  @author Emiel Bruijntjes
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class HashIterator : public ValueIteratorImpl
{
public:
    /**
     *  Constructor
     *  @param  hashtable       The hashtable to iterate over
     *  @param  first           Should it start on the first position?
     *  @param  tsrm_ls
     */
    HashIterator(HashTable *hashtable, bool first, bool is_array = false) : _table(hashtable), _is_array(is_array)
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
     *  Copy constructor
     *  @param  that
     *  @param  tsrm_ls
     */
    HashIterator(const HashIterator &that TSRMLS_DC) :
        _table(that._table), _position(that._position), _is_array(that._is_array)
    {
        // read current position
        read();
    }
    
    /**
     *  Destructor
     */
    virtual ~HashIterator() {}
    
    /**
     *  Clone the object
     *  @param  tsrm_ls
     *  @return ValueIteratorImpl
     */
    virtual ValueIteratorImpl *clone()
    {
        // create a new instance
        return new HashIterator(*this);
    }

    /**
     *  Increment position (pre-increment)
     *  @return bool
     */
    virtual bool increment() override
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
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() override
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
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    virtual bool equals(const ValueIteratorImpl *that) const override
    {
        // this always is a hash iterator
        HashIterator *other = (HashIterator *)that;
        
        // compare the positions
        return _position == other->_position;
    }

    /**
     *  Derefecence, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    virtual const std::pair<Value,Value> &current() const override
    {
        return _current;
    }

private:
    /**
     *  The hash table over which is being iterated
     *  @var    HashTable
     */
    HashTable *_table = nullptr;

    /**
     *  The position in the hash table
     *  @var    HashPosition
     */
    Bucket *_position = nullptr;

    /**
     * Is a hash interator in array
     * @var     bool
     */
     bool _is_array = false;

    /**
     *  The current key and value
     *  @var    std::pair
     */
    std::pair<Value,Value> _current;

    /**
     *  Read current key and value
     *  @return bool
     */
    bool read()
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
        else key = std::string(string_key, str_len - 1);

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
        return _is_array || !_current.first.isString() || _current.first.rawValue()[0];
    }

    /**
     *  Invalidate the iterator
     *  @return bool
     */
    bool invalidate()
    {
        // forget current position
        _position = nullptr;
        
        // make the data a pair of null ptrs
        _current = std::make_pair<Value,Value>(nullptr,nullptr);
        
        // done
        return false;
    }
};

/**
 *  End namespace
 */
}

