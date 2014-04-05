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
private:
    /**
     *  Unique pointer to the iterator that is returned by the extension
     *  @var    std::unique_ptr
     */
    std::unique_ptr<Iterator> _iterator;

    /**
     *  The current() method that is called by the Zend engine wants a 
     *  pointer-to-pointer-to-a-zval. Because of this, we have to keep the 
     *  current value in memory after the current() method returns because 
     *  the pointer would otherwise fall out of scope. This is (once again)
     *  odd behavior of the Zend engine, but we'll have to live with that
     *  @var    Value
     */
    Value _current;

    /**
     *  The object iterator as is needed by the Zend engine
     *  @var    zend_object_iterator
     */
    zend_object_iterator _impl;

    /**
     *  Get access to all iterator functions
     *  @return zend_object_iterator_funcs
     */
    static zend_object_iterator_funcs *functions();

    /**
     *  Is the iterator on a valid position
     *  @return bool
     */
    bool valid()
    {
        return _iterator->valid();
    }
    
    /**
     *  The value at the current position
     *  @return Value
     */
    Value current()
    {
        return _iterator->current();
    }
    
    /**
     *  The key at the current position
     *  @return Value
     */
    Value key()
    {
        return _iterator->key();
    }
    
    /**
     *  Move to the next position
     */
    void next()
    {
        return _iterator->next();
    }
    
    /**
     *  Rewind the iterator to the front position
     */
    void rewind()
    {
        return _iterator->rewind();
    }
    
    /**
     *  Iterator destructor method
     *  @param  iter
     *  @param  tsrm_ls
     */
    static void destructor(zend_object_iterator *iter TSRMLS_DC);

    /**
     *  Iterator valid function
     *  Returns FAILURE or SUCCESS
     *  @param  iter
     *  @param  tsrm_ls
     *  @return int
     */
    static int valid(zend_object_iterator *iter TSRMLS_DC);

    /**
     *  Fetch the current item
     *  @param  iter
     *  @param  data
     *  @param  tsrm_ls
     */
    static void current(zend_object_iterator *iter, zval ***data TSRMLS_DC);

    /**
     *  Fetch the key for the current element (optional, may be NULL). The key
     *  should be written into the provided zval* using the ZVAL_* macros. If
     *  this handler is not provided auto-incrementing integer keys will be
     *  used.
     *  @param  iter
     *  @param  data
     *  @param  tsrm_ls
     */
    static void key(zend_object_iterator *iter, zval *data TSRMLS_DC);

    /**
     *  Function to retrieve the current key, php 5.3 style
     *  @param  iter
     *  @param  str_key
     *  @param  str_key_len
     *  @param  int_key
     *  @param  tsrm_ls
     *  @return HASH_KEY_IS_STRING or HASH_KEY_IS_LONG
     */
    static int key(zend_object_iterator *iter, char **str_key, unsigned int *str_key_len, unsigned long *int_key TSRMLS_DC);

    /**
     *  Step forwards to the next element
     *  @param  iter
     *  @param  tsrm_ls
     */
    static void next(zend_object_iterator *iter TSRMLS_DC);

    /**
     *  Rewind the iterator back to the start
     *  @param  iter
     *  @param  tsrm_ls
     */
    static void rewind(zend_object_iterator *iter TSRMLS_DC);

public:
    /**
     *  Constructor
     *  @param  iterator        The iterator that is implemented by the extension
     */
    IteratorImpl(Iterator *iterator) : _iterator(iterator) 
    {
        // initialize impl object
        _impl.data = this;
        _impl.index = 0;
        _impl.funcs = functions();
    }
    
    /**
     *  Destructor
     */
    virtual ~IteratorImpl() {}

    /**
     *  Internal method that returns the implementation object
     *  @return zend_object_iterator
     */
    zend_object_iterator *implementation()
    {
        return &_impl;
    }
};
    
/**
 *  End namespace
 */
}
