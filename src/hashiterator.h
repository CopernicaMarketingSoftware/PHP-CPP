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
class HashIterator : public IteratorImpl
{
public:
    /**
     *  Constructor
     *  @param  hashtable       The hashtable to iterate over
     *  @param  first           Should it start on the first position?
     */
    HashIterator(struct _hashtable *hashtable, bool first);
    
    /**
     *  Copy constructor
     *  @param  that
     */
    HashIterator(const ValueIterator &that);
    
    /**
     *  Destructor
     */
    virtual ~HashIterator() {}

    /**
     *  Increment position (pre-increment)
     *  @return bool
     */
    virtual bool increment() override;
    
    /**
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() override;

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

