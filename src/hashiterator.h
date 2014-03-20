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
class HashIterator : public IteratorImpl
{
public:
    /**
     *  Constructor
     *  @param  hashtable       The hashtable to iterate over
     *  @param  first           Should it start on the first position?
     */
    HashIterator(HashTable *hashtable, bool first) : _table(hashtable)
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
     */
    HashIterator(const HashIterator &that) :
        _table(that._table), _position(that._position)
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
     *  @return IteratorImpl
     */
    virtual IteratorImpl *clone()
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
    virtual bool equals(const IteratorImpl *that) const override
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
     *  The current key and value
     *  @var    std::pair
     */
    std::pair<Value,Value> _current;

    /**
     *  Read current key and value
     *  @return bool
     */
    bool read();

    /**
     *  Invalidate the iterator
     *  @return bool
     */
    bool invalidate();
};

/**
 *  End namespace
 */
}

