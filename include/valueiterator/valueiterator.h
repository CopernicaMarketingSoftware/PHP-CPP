/**
 *  valueiterator.h
 *
 *  ValueIterator - allows the iteration variable of type Value.
 *  It designed for natural iteration objects, arrays and other iterable types. And have minimal overhead.
 *
 *  @copyright 2013 Copernica BV
 */


/**
 *  Set up namespace
 */
namespace Php {


class ValueIterator
{
public:
    
    /**
     *  Constructor for empty ValueIterator.
     *  Used to finish the iterations
     */
    ValueIterator(std::nullptr_t n) : phItem(nullptr) {}

    /**
     *  Constructor ValueIterator
     *  @param  arr HashTable
     */
    ValueIterator(HashItem *phi): phItem(phi)
    {
        phi->reset();
    }

    /**
     *  Increment prefix operator
     */
    ValueIterator& operator++() {
        phItem->next();
        return *this;
    }

    /**
     *  Increment postfix operator
     */
    ValueIterator operator++(int) {
        ValueIterator tmp(*this);
        operator++();
        return tmp;
    }

    /**
     *  compare operator
     */
    bool operator==(const ValueIterator& rhs) const {
        // If one of items is empty
        // The order of the following tests is optimized. Do not change it.
        if(!isEmpty() &&  rhs.isEmpty() ) return false;
        if( isEmpty() &&  rhs.isEmpty() ) return true;
        if( isEmpty() && !rhs.isEmpty() ) return false;

        // If both are not empty
        return phItem->compare(rhs.phItem);
    }

    bool operator!=(const ValueIterator& rhs) const {
        return !operator==(rhs);
    }

    HashItem& operator*() {
        return *phItem;
    }
    
    HashItem* operator->() {
        return phItem;
    }

private:

    /**
     *  Check the current item is empty
     */
    bool isEmpty() const
    {
        // True, if phItem == nullptr or  phItem is empty.
        return (!phItem || phItem->isEmpty());
    }

    // Position in the internal hash table. Pointer to HashItem
    HashItem *phItem;
};

/**
 *  End of namespace
 */
}
