/**
 *  valueiterator.h
 *
 *  ValueIterator - designed for natural iteration objects and arrays of type Value. 
 *  it does not use STL containers and carries minimal overhead.
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef VALUEITERATOR_H
#define VALUEITERATOR_H

/**
 *  Forward declaration
 */
struct _hashtable;

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
     *  Constructor for empty ValueIterator.
     *  Used only for build ValueIterator::null
     */
    ValueIterator() {
        _pair.reset();
    }

    /**
     *  Constructor ValueIterator
     *  @param  arr HashTable
     */
    ValueIterator(_hashtable *arr, bool isArray);

    /**
     *  Copy Constructor
     */
    ValueIterator(const ValueIterator& that) : _arr(that._arr), _pair(that._pair), _isArray(that._isArray) {}

    /**
     *  Move Constructor
     */
    ValueIterator(ValueIterator&& that) :  _arr(std::move(that._arr)), _pair(std::move(that._pair)), _isArray(that._isArray) {}

    /**
     *  Increment operator
     */
    ValueIterator& operator++() {
        nextIteration();
        setPair();
        return *this;
    }

    ValueIterator operator++(int) {
        ValueIterator tmp(*this);
        operator++();
        return tmp;
    }

    bool operator==(const ValueIterator& rhs) const {
        return (_pair._ind == rhs._pair._ind && _pair._key == rhs._pair._key );
    }

    bool operator!=(const ValueIterator& rhs) const {
        return !operator==(rhs);
    }

    ValuePair& operator*() {
        return _pair;
    }
    
    ValuePair* operator ->() {
        return &_pair;
    }

    // Empty iterator. Used to finish the iterations
    static ValueIterator null;

private:

    /**
     *  Retrieving data from `HashTable arr` and filling in the `ValuePair _pair`
     */
    void setPair();

    /**
     *  next iteration
     */
    void nextIteration();
    
    // ValueIterator always used through ValuePair
    ValuePair _pair;

    // get access to the hast table (HashTable)
    _hashtable *_arr;

    // is Array? (true — yes, false — no, is Object)
    bool _isArray;
    
};

/**
 *  End of namespace
 */
}
#endif /* valueiterator.h */
