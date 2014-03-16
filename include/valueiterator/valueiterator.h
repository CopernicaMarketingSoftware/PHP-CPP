/**
 *  valueiterator.h
 *
 *  ValueIterator - allows the iteration variable of class Value.
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
     *  Constructor ValueIterator
     *  @param  arr HashTable
     */
    ValueIterator(HashItem *phi): ItemPos(phi)
    {
        phi->reset();
    }
    
    /**
     *  Constructor for empty ValueIterator.
     *  Used to finish the iterations
     */
    ValueIterator(std::nullptr_t n) : ItemPos(nullptr) {}

    /**
     *  Copy Constructor
     */
    ValueIterator(const ValueIterator& that) = delete;

    /**
     *  Move constructor
     */
    ValueIterator(ValueIterator&& that)
    {
        ItemPos = that.ItemPos;
        // clear the other object
        that.ItemPos = nullptr;
    }

    /**
     *  Move assignment
     */
    Value &operator=(ValueIterator&& that)
    {
        ItemPos = that.ItemPos;
        // clear the other object
        that.ItemPos = nullptr;
    }

    /**
     *  Increment prefix operator
     */
    ValueIterator& operator++()
    {
        next();
        return *this;
    }

    /**
     *  Increment postfix operator
     */
    ValueIterator& operator++(int)
    {
        return operator++();
    }

    /**
     *  Add a int-value to the ValueIterator-object
     */
    ValueIterator& operator+=(unsigned int n)
    {
        for(unsigned int i=0; i < n; ++i) next();
        return *this;
    }

    bool operator==(const ValueIterator& rhs) const
    {
        // If one of items is empty
        // The order of the following tests is optimized. Do not change it.
        if(!isEmpty() &&  rhs.isEmpty() ) return false;
        if( isEmpty() &&  rhs.isEmpty() ) return true;
        if( isEmpty() && !rhs.isEmpty() ) return false;

        // If both are not empty
        return ItemPos->compare(rhs.ItemPos);
    }

    bool operator!=(const ValueIterator& rhs) const
    {
        return !operator==(rhs);
    }

    HashItem& operator*()
    {
        return *ItemPos;
    }
    
    HashItem* operator->()
    {
        return ItemPos;
    }

    /**
     *  Destructor
     */
    ~ValueIterator()
    {
        // delete HashItem pointer
        if(ItemPos) delete ItemPos;
    }


private:

    /**
     *  Check the current item is empty
     */
    bool isEmpty() const
    {
        // True, if ItemPos == nullptr or  ItemPos is empty.
        return (!ItemPos || ItemPos->isEmpty());
    }

    /**
     *  next iteration
     */
    bool next()
    {
        // Only if ItemPos != nullptr
        if(ItemPos) ItemPos->next();
    }

    /**
     *  HashItem pointer
     *  Pointer to a position in the internal array/oblect/traversable.
     *  (if the internal array does not exist, then nullptr).
     *  @var HashItem*
     */
    HashItem *ItemPos;
};

/**
 *  End of namespace
 */
}
