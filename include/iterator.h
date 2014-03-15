/**
 *  Iterator.h
 *
 *  Base class for iterators. Extension writers that want to create traversable
 *  classes, should override this class and implement all pure virtual methods
 *  in it.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_object_iterator_funcs;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Iterator
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
    
private:
    /**
     *  During the lifetime of the iterator, the object over which
     *  it iterates is keps as a private variable. This ensures that
     *  this object is not destructed as long as the iterator exists
     *  @var    Value
     */
    Value _object;
    
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
     *  Internal method that returns the implementation object
     *  @return zend_object_iterator
     */
    struct _zend_object_iterator *implementation();
    
    /**
     *  Iterator destructor method
     *  @param  iter
     */
    static void destructor(struct _zend_object_iterator *iter);

    /**
     *  Iterator valid function
     *  Returns FAILURE or SUCCESS
     *  @param  iter
     *  @return int
     */
    static int valid(struct _zend_object_iterator *iter);

    /**
     *  Fetch the current item
     *  @param  iter
     *  @param  data
     */
    static void current(struct _zend_object_iterator *iter, struct _zval_struct ***data);

    /**
     *  Fetch the key for the current element (optional, may be NULL). The key
     *  should be written into the provided zval* using the ZVAL_* macros. If
     *  this handler is not provided auto-incrementing integer keys will be
     *  used.
     *  @param  iter
     *  @param  data
     */
    static void key(struct _zend_object_iterator *iter, struct _zval_struct *data);

    /**
     *  Function to retrieve the current key, php 5.3 style
     *  @param  iter
     *  @param  str_key
     *  @param  str_key_len
     *  @param  int_key
     *  @return HASH_KEY_IS_STRING or HASH_KEY_IS_LONG
     */
    static int key(struct _zend_object_iterator *iter, char **str_key, unsigned int *str_key_len, unsigned long *int_key);

    /**
     *  Step forwards to the next element
     *  @param  iter
     */
    static void next(struct _zend_object_iterator *iter);

    /**
     *  Rewind the iterator back to the start
     *  @param  iter
     */
    static void rewind(struct _zend_object_iterator *iter);

    /**
     *  Get access to all iterator functions
     *  @return zend_object_iterator_funcs
     */
    static struct _zend_object_iterator_funcs *functions();

    /**
     *  Classbase is a friend
     */
    friend class ClassBase;
};
    
/**
 *  End namespace
 */
}
