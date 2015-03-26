/**
 *  Traversable.h
 *
 *  Interface that should be implemented by classes that want to be
 *  traversable. A traversable objects can be iterated over in a PHP
 *  foreach loop.
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
class PHPCPP_EXPORT Traversable
{
public:
    /**
     *  Retrieve an instance of the iterator
     *  @return Iterator
     */
    virtual Iterator *getIterator() = 0;
};

/**
 *  End namespace
 */
}
