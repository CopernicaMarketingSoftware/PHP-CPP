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
     */
    HashIterator(HashTable *hashtable, bool first, bool is_array = false) : _table(hashtable), _is_array(is_array)
    {
        // reset the hash pointer to the internal position
        if (hashtable && first)
        {
            // we should be valid (this is undone later if necessary)
            _valid = true;

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
     */
    HashIterator(const HashIterator &that) :
        _table(that._table), _position(that._position), _is_array(that._is_array)
    {
        // read current position
        read();
    }

    /**
     *  Destructor
     */
    virtual ~HashIterator() = default;

    /**
     *  Clone the object
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
        // leap out if we're not even iterating over a hash table
        if (!_table) return false;

        // leap out if already on an invalid pos (behind the last pos)
        if (!_valid) return false;

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
        if (!_valid)
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

        // compare the tables and positions
        return _table == other->_table && _position == other->_position;
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
     *  Are we at a possibly valid position?
     *  @var    bool
     */
    bool _valid = false;

    /**
     *  The hash table over which is being iterated
     *  @var    HashTable
     */
    HashTable *_table = nullptr;

    /**
     *  The position in the hash table
     *  @var    HashPosition
     */
    HashPosition _position;

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

        // read in the current key
        zend_hash_get_current_key_zval_ex(_table, key._val, &_position);

        // if the key is set to NULL, it means that the object is not at a valid position
        if (key.isNull()) return invalidate();

        // iterator is at a valid position, go fetch the data
        auto *value = zend_hash_get_current_data_ex(_table, &_position);

        // we can now update the current data
        _current = std::make_pair<Value,Value>(std::move(key), value);

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
        // set position to be one after the end
        zend_hash_internal_pointer_end_ex(_table, &_position);
        zend_hash_move_forward_ex(_table, &_position);

        // no longer valid
        _valid = false;

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

