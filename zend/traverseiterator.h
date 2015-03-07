/**
 *  TraverseIterator.h
 *
 *  When an object from PHP userspace implements its own iterator methods,
 *  and the Php::Value object is used to iterate over its properties, this
 *  TraversableIterator class is used to iterate over the properties
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
class TraverseIterator : public ValueIteratorImpl
{
public:
    /**
     *  Constructor
     *  @param  object
     *  @param  begin
     *  @param  tsrm_ls
     */
    TraverseIterator(zval *object, bool begin TSRMLS_DC) : _object(object)
    {
        // leap out if this iterator starts at the end
        if (!begin) return;
        
        // we need the class entry
        auto *entry = zend_get_class_entry(object TSRMLS_CC);
        
        // create the iterator
        _iter = entry->get_iterator(entry, object, false TSRMLS_CC);
        
        // rewind the iterator
        _iter->funcs->rewind(_iter TSRMLS_CC);
        
        // read the first key/value pair
        read(TSRMLS_C);
    }
    
    /**
     *  Copy constructor
     *  @param  that
     *  @param  tsrm_ls
     */
    TraverseIterator(const TraverseIterator &that TSRMLS_DC) : TraverseIterator(that._object, that._iter != nullptr TSRMLS_CC)
    {
        // @todo    this is a broken implementation, the copy is at the start
        //          position, while we'd like to be at the same position
    }
    
    /**
     *  Destructor
     */
    virtual ~TraverseIterator()
    {
        // do nothing if iterator is already invalid
        if (!_iter) return;
        
        // we need the tsrm pointer
        TSRMLS_FETCH();
        
        // call the iterator destructor
        if (_iter) _iter->funcs->dtor(_iter TSRMLS_CC);
    }

    /**
     *  Clone the object
     *  @param  tsrm_ls
     *  @return ValueIteratorImpl*
     */
    virtual ValueIteratorImpl *clone() override
    {
        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        
        // construct iterator
        return new TraverseIterator(*this TSRMLS_CC);
    }

    /**
     *  Increment position (pre-increment)
     *  @param  tsrm_ls
     *  @return bool
     */
    virtual bool increment() override
    {
        // do we still have an iterator?
        if (!_iter) return false;

        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        
        // movw it forward
        _iter->funcs->move_forward(_iter TSRMLS_CC);
        
        // and read current data
        read(TSRMLS_C);
        
        // done
        return true;
    }
    
    /**
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() override
    {
        // not possible with PHP iterators
        throw Exception("Impossible to iterate backwards");
        
        // unreachable
        return false;
    }

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    virtual bool equals(const ValueIteratorImpl *that) const override
    {
        // of course if the objects are identical
        if (this == that) return true;
        
        // cast to traverse-iterator
        TraverseIterator *other = (TraverseIterator *)that;
        
        // if both objects are in an invalid state we consider them to be identical
        if (!_iter && !other->_iter) return true;
        
        // although the iterators could be at the same pos, for simplicity
        // we consider them different here
        return false;
    }

    /**
     *  Derefecence, this returns a std::pair with the current key and value
     *  @return std::pair
     */
    virtual const std::pair<Value,Value> &current() const
    {
        return _data;
    }

private:
    /**
     *  The object that is iterated over
     *  @var    _val
     */
    zval *_object = nullptr;
    
    /**
     *  The iterator from Zend
     *  @var    zend_object_iterator
     */
    struct _zend_object_iterator *_iter = nullptr;

    /**
     *  Current data
     *  @var    pair
     */
    std::pair<Value,Value> _data;


    /**
     *  Read current data
     *  @param  tsrm_ls
     *  @return bool
     */
    bool read(TSRMLS_D)
    {
        // not possible when no iterator exists
        if (!_iter) return false;

        // is the iterator at a valid position?
        if (_iter->funcs->valid(_iter TSRMLS_CC) == FAILURE) return invalidate(TSRMLS_C);

#if PHP_VERSION_ID >= 50500

        // create a value object
        Value val;
        
        // call the function to get the key
        _iter->funcs->get_current_key(_iter, val._val TSRMLS_CC);
        
        // store the key
        _data.first = val;

#else

        // variable we need for fetching the key, and that will be assigned by
        // the PHP engine (this is php 5.3 code)
        char *str_key; unsigned int str_key_len; unsigned long int_key;

        // php 5.4 or php 5.3 code, fetch the current key
        int type = _iter->funcs->get_current_key(_iter, &str_key, &str_key_len, &int_key TSRMLS_CC);
        
        // what sort of key do we have?
        if (type == HASH_KEY_IS_LONG) 
        {
            // we have an int key
            _data.first = (int64_t)int_key;
        }
        else
        {
            // we have a string key that is already allocated
            _data.first = str_key;
            
            // deallocate the data
            efree(str_key);
        }

#endif

        // now we're going to fetch the value, for this we need a strange zval
        // it is strange that this is a pointer-to-pointer, but that is how
        // the Zend engine implements this. It is going to be filled with
        // a pointer to a memory address that is guaranteed to hold a valid
        // zval.
        zval **zval;
        
        // get the current value
        _iter->funcs->get_current_data(_iter, &zval TSRMLS_CC);
        
        // wrap the zval in a value object
        _data.second = Value(*zval);
     
        // done
        return true;
    }
    
    /**
     *  Invalidate the object
     *  @param  tsrm_ls
     *  @return bool
     */
    bool invalidate(TSRMLS_D)
    {
        // skip if already invalid
        if (!_iter) return false;
        
        // reset the iterator
        _iter->funcs->dtor(_iter TSRMLS_CC);
        
        // set back to null
        _iter = nullptr;
        
        // done
        return false;
    }
};

/**
 *  End namespace
 */
}

