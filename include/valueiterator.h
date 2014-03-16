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
     *  Destructor
     */
    virtual ~ValueIterator() {}

    /**
     *  Increment position
     *  @return ValueIterator
     */
    ValueIterator &operator++();
    
    /**
     *  Decrement position
     *  @return ValueIterator
     */
    ValueIterator &operator--();
    
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
     *  @return ValueIterator
     */
    ValueIterator &read();

    /**
     *  Invalidate the iterator
     *  @return ValueIterator
     */
    ValueIterator &invalidate();
};

/**
 *  End namespace
 */
}

