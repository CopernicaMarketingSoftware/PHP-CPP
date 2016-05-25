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
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declarations
 */
class ValueIteratorImpl;

/**
 *  Class definition
 */
class PHPCPP_EXPORT ValueIterator
{
public:
    /**
     *  Constructor
     *  @param  impl        Implementation iterator
     */
    ValueIterator(ValueIteratorImpl *impl);

    /**
     *  Copy constructor
     *  @param  that
     */
    ValueIterator(const ValueIterator &that);

    /**
     *  Destructor
     */
    virtual ~ValueIterator();

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
    bool operator==(const ValueIterator &that) const;

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    bool operator!=(const ValueIterator &that) const;

    /**
     *  Dereference, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    const std::pair<Value,Value> &operator*() const;

    /**
     *  Dereference, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    const std::pair<Value,Value> *operator->() const;

private:
    /**
     *  Pointer to the actual implementation
     *  @var    std::unique_ptr<ValueIteratorImpl>
     */
    std::unique_ptr<ValueIteratorImpl> _impl;

};

/**
 *  End namespace
 */
}
