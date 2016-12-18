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
class IteratorImpl
{
public:
    /**
     *  Get access to all iterator functions
     *  @return zend_object_iterator_funcs
     */
    static zend_object_iterator_funcs *functions();

private:
    /**
     *  The normal zend_object_iterator
     *  @var zend_object_iterator
     */
    zend_object_iterator _iterator;

    /**
     *  Unique pointer to the user space iterator that is returned by the extension
     *  @var    std::unique_ptr
     */
    std::unique_ptr<Iterator> _userspace;

    /**
     *  Current value
     *  @var Value
     */
    Value _current;

    /**
     *  Is the iterator on a valid position
     *  @return bool
     */
    bool valid()
    {
        return _userspace->valid();
    }

    /**
     *  The value at the current position
     *  @return Value
     */
    Value &current()
    {
        // get from the user space iterator, and store as member
        return _current = _userspace->current();
    }

    /**
     *  The key at the current position
     *  @return Value
     */
    Value key()
    {
        return _userspace->key();
    }

    /**
     *  Move to the next position
     */
    void next()
    {
        return _userspace->next();
    }

    /**
     *  Rewind the iterator to the front position
     */
    void rewind()
    {
        return _userspace->rewind();
    }
    
    /**
     *  Invalidate the current variable
     */
    void invalidate()
    {
        _current.invalidate();
    }

    /**
     *  Helper method to get access to ourselves
     *  @param  iter
     *  @return IteratorImpl
     */
    static IteratorImpl *self(zend_object_iterator *iter);

    /**
     *  Iterator destructor method
     *  @param  iter
     */
    static void destructor(zend_object_iterator *iter);

    /**
     *  Iterator valid function
     *  Returns FAILURE or SUCCESS
     *  @param  iter
     *  @return int
     */
    static int valid(zend_object_iterator *iter);

    /**
     *  Fetch the current item
     *
     *  @param  iter    The iterator used to retrieve the value from
     *  @return The current value of the iterator
     */
    static zval *current(zend_object_iterator *iter);

    /**
     *  Fetch the key for the current element (optional, may be NULL). The key
     *  should be written into the provided zval* using the ZVAL_* macros. If
     *  this handler is not provided auto-incrementing integer keys will be
     *  used.
     *  @param  iter
     *  @param  data
     */
    static void key(zend_object_iterator *iter, zval *data);

    /**
     *  Step forwards to the next element
     *  @param  iter
     */
    static void next(zend_object_iterator *iter);

    /**
     *  Rewind the iterator back to the start
     *  @param  iter
     */
    static void rewind(zend_object_iterator *iter);
    
    /**
     *  Invalidate current object
     *  @param  iter
     */
    static void invalidate(zend_object_iterator *iter);

public:
    /**
     *  Constructor
     *  @param  zval            The object that is being iterated
     *  @param  iterator        The iterator that is implemented by the extension
     */
    IteratorImpl(zval *object, Iterator *iterator);

    /**
     *  Destructor
     *  Important: this should not be virtual because this object is not allowed
     *  to have a vtable because it replies of memory alignment of the first member.
     */
    ~IteratorImpl();

    /**
     *  Internal method that returns the implementation object
     *  @return zend_object_iterator
     */
    zend_object_iterator *implementation()
    {
        return &_iterator;
    }
};

/**
 *  End namespace
 */
}
