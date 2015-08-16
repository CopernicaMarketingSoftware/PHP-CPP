/**
 *  @file arrayaccess.h
 *
 *  This file provides an "Interface" that can be "implemented" by your class.
 *
 *  If you do, you create your class as follows:
 *     class MyClass : public Php::Base, public Php::ArrayAccess { ... }
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
class PHPCPP_EXPORT ArrayAccess
{
public:
    /**
     *  Check if an element exists by supplying its key to find it
     *
     *  @param  key  - Key to find the associated element
     *
     *  @return bool - true if the element exist, false otherwise
     */
    virtual bool offsetExists(const Php::Value &key) = 0;

    /**
     *  Set a slot within the array with the specified key-value pair
     *
     *  @param key   - Key to find the associated value
     *  @param value - The value to associate with the key
     */
    virtual void offsetSet(const Php::Value &key, const Php::Value &value) = 0;

    /**
     *  Retrieve a copy of the value associated with the specified key
     *
     *  @param  key   - Key to find the associated value
     *
     *  @return value - The value associated with the key
     */
    virtual Php::Value offsetGet(const Php::Value &key) = 0;

    /**
     *  Remove an element from the array associated with the specified key
     *
     *  @param key - Key to find the associated element
     */
    virtual void offsetUnset(const Php::Value &key) = 0;

    /**
     * Default constructor
     */
     constexpr
     explicit ArrayAccess() _NOEXCEPT =default;

    /**
     * Default destructor
     */
    virtual ~ArrayAccess() _NOEXCEPT =default;
};

/**
 *  End namespace
 */
}
