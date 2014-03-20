/**
 *  IteratorImpl.h
 *
 *  Interface that describes what an implementation of an iterator should
 *  look like. This is an internal class that extension developers do not
 *  need.
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
class IteratorImpl
{
public:
    /**
     *  Clone the object
     *  @return IteratorImpl*
     */
    virtual IteratorImpl *clone() = 0;

    /**
     *  Increment position (pre-increment)
     *  @return bool
     */
    virtual bool increment() = 0;
    
    /**
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() = 0;

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    virtual bool equals(const IteratorImpl *that) const = 0;

    /**
     *  Derefecence, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    virtual const std::pair<Value,Value> &current() const = 0;
};    
    
/**
 *  End namespace
 */
}

