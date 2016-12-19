/**
 *  InvalidIterator.h
 *
 *  Iterator class that is used for value objects that are not even
 *  iteratable.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class InvalidIterator : public ValueIteratorImpl
{
public:
    /**
     *  Clone the object
     *  @return ValueIteratorImpl
     */
    virtual ValueIteratorImpl *clone()
    {
        // create a new instance
        return new InvalidIterator(*this);
    }

    /**
     *  Increment position (pre-increment)
     *  @return bool
     */
    virtual bool increment() override
    {
        return false;
    }
    
    /**
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() override
    {
        return false;
    }

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    virtual bool equals(const ValueIteratorImpl *that) const override
    {
        // the other iterator is also an invalid-iterator, and all invalid
        // iterators are equal
        return true;
    }

    /**
     *  Derefecence, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    virtual const std::pair<Value,Value> &current() const override
    {
        // this method is never called, when it is, we create a static instance
        static std::pair<Value,Value> result;
    
        // return it
        return result;
    }
};

/**
 *  End namespace
 */
}

