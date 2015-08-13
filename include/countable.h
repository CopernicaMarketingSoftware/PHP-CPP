/**
 *  @file countable.h
 *
 *  This file provides an "Interface" that can be "implemented" by your class.
 *
 *  If you do, you create your class as follows:
 *
 *      class MyClass : public Php::Base, public Php::Countable { ... }
 *
 *  You will have to implement the Php::Countable::count() method, which should
 *  return the number of elements in the object
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *
 *  @copyright 2014-2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Countable
{
public:
    /**
     *  Method to retrieve the number of items in the class
     *
     *  @return long - The number of items in the class
     */
    virtual long count() = 0;

    /**
     *  Default destructor
     */
    virtual ~Countable() _NOEXCEPT = default;
};

/**
 *  End namespace
 */
}
