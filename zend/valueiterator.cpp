/**
 *  ValueIterator.cpp
 *
 *  Implementation of the value iterator
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  impl        Implementation iterator
 */
ValueIterator::ValueIterator(ValueIteratorImpl *impl) : _impl(impl) {}

/**
 *  Copy constructor
 *  @param  that
 */
ValueIterator::ValueIterator(const ValueIterator &that) : _impl(that._impl->clone()) {}

/**
 *  Destructor
 */
ValueIterator::~ValueIterator() = default;

/**
 *  Increment position
 *  @return ValueIterator
 */
ValueIterator &ValueIterator::operator++()
{
    // increment implementation
    _impl->increment();

    // done
    return *this;
}

/**
 *  Decrement position
 *  @return ValueIterator
 */
ValueIterator &ValueIterator::operator--()
{
    // decrement implementation
    _impl->decrement();

    // done
    return *this;
}

/**
 *  Compare with other iterator
 *  @param  that
 *  @return bool
 */
bool ValueIterator::operator==(const ValueIterator &that) const
{
    return _impl->equals(that._impl.get());
}

/**
 *  Compare with other iterator
 *  @param  that
 *  @return bool
 */
bool ValueIterator::operator!=(const ValueIterator &that) const
{
    return !_impl->equals(that._impl.get());
}

/**
 *  Derefecence, this returns a std::pair with the current key and value
 *  @return std::pair
 */
const std::pair<Value,Value> &ValueIterator::operator*() const
{
    return _impl->current();
}

/**
 *  Dereference, this returns a std::pair with the current key and value
 *  @return std::pair
 */
const std::pair<Value,Value> *ValueIterator::operator->() const
{
    return &_impl->current();
}

/**
 *  End namespace
 */
}

