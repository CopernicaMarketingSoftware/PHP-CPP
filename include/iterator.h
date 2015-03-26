/**
 *  Iterator.h
 *
 *  Base class for iterators. Extension writers that want to create traversable
 *  classes, should override the Php::Traversable base class. This base class
 *  forces you to implement a getIterator() method that returns an instance of
 *  a Php::Iterator class.
 *
 *  In this file you find the signature of the Php::Iterator class. It mostly has
 *  pure virtual methods, which means that you should create a derived class
 *  that implements all these methods.
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
class PHPCPP_EXPORT Iterator
{
public:
    /**
     *  Constructor
     *  @param  base        Class over which the iterator is iterating
     */
    Iterator(Base *base) : _object(base) {}

    /**
     *  Destructor
     */
    virtual ~Iterator() {}

    /**
     *  Is the iterator on a valid position
     *  @return bool
     */
    virtual bool valid() = 0;

    /**
     *  The value at the current position
     *  @return Value
     */
    virtual Value current() = 0;

    /**
     *  The key at the current position
     *  @return Value
     */
    virtual Value key() = 0;

    /**
     *  Move to the next position
     */
    virtual void next() = 0;

    /**
     *  Rewind the iterator to the front position
     */
    virtual void rewind() = 0;

protected:
    /**
     *  During the lifetime of the iterator, the object over which
     *  it iterates is keps as a private variable. This ensures that
     *  this object is not destructed as long as the iterator exists
     *  @var    Value
     */
    Value _object;

};

/**
 *  End namespace
 */
}
