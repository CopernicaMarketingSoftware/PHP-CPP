/**
 *  ValueIterator.h
 *
 *  This is an internal helper class that is used when iterating over a
 *  Php::Value object - stl style.
 *
 *  Thus, when you do c++ things like "for (auto &iter : value)", internally
 *  a ValueIterator object is being used.
 *
 *  @author Emiel Bruijntjes
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declaration
 */
struct _hashtable;
struct bucket;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ValueIterator
{
public:
    /**
     *  Constructor
     *  @param  hashtable       The hashtable to iterate over
     *  @param  first           Should it start on the first position?
     */
    ValueIterator(struct _hashtable *hashtable, bool first);
    
    /**
     *  Copy constructor
     *  @param  that
     */
    ValueIterator(const ValueIterator &that);
    
    /**
     *  Destructor
     */
    virtual ~ValueIterator() {}

    /**
     *  Increment position (pre-increment)
     *  @return ValueIterator
     */
    ValueIterator &operator++();
    
    /**
     *  Increment position (post-increment)
     *  @return ValueIterator
     */
    ValueIterator operator++(int)
    {
        // make a copy
        ValueIterator copy(*this);
        
        // increment current object
        ++(*this);
        
        // and return the unchanged original
        return copy;
    }
    
    /**
     *  Decrement position (pre-decrement)
     *  @return ValueIterator
     */
    ValueIterator &operator--();

    /**
     *  Increment position (post-decrement)
     *  @return ValueIterator
     */
    ValueIterator operator--(int)
    {
        // make a copy
        ValueIterator copy(*this);
        
        // decrement current object
        --(*this);
        
        // and return the unchanged original
        return copy;
    }
    
    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    bool operator==(const ValueIterator &that) const
    {
        return _position == that._position;
    }

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    bool operator!=(const ValueIterator &that) const
    {
        return _position != that._position;
    }

    /**
     *  Derefecence, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    const std::pair<Value,Value> &operator*() const
    {
        return _current;
    }
    
    /**
     *  Dereference, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    const std::pair<Value,Value> *operator->() const
    {
        return &_current;
    }

private:
    /**
     *  The hash table over which is being iterated
     *  @var    HashTable
     */
    struct _hashtable *_table = nullptr;

    /**
     *  The position in the hash table
     *  @var    HashPosition
     */
    struct bucket *_position = nullptr;
    
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

