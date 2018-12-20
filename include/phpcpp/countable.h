/**
 *  Countable.h
 *
 *  "Interface" that can be "implemented" by your class. If you do, you
 *  create your class like this:
 *
 *  class MyClass : public Php::Base, public Php::Countable { ... }
 *
 *  You will have to implement the count() method, which should return the
 *  number of elements in the object
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
class PHPCPP_EXPORT Countable
{
public:
    /**
     *  Retrieve the number of items in the class
     *  @return long
     */
    virtual long count() = 0;

};

/**
 *  End namespace
 */
}
