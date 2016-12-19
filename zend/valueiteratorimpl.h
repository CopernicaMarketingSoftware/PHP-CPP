/**
 *  ValueIteratorImpl.h
 *
 *  Interface that describes what an implementation of a value iterator should
 *  look like. This is an internal class that extension developers do not
 *  need. It is used internally inside the ValueIterator class.
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
class ValueIteratorImpl
{
public:
    /**
     *  Constructor
     */
    ValueIteratorImpl() {}
    
    /**
     *  Destructor
     */
    virtual ~ValueIteratorImpl() {}

    /**
     *  Clone the object
     *  @return ValueIteratorImpl*
     */
    virtual ValueIteratorImpl *clone() = 0;

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
    virtual bool equals(const ValueIteratorImpl *that) const = 0;

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

