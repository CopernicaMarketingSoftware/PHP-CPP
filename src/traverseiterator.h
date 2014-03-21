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
class TraverseIterator : public IteratorImpl
{
public:
    /**
     *  Constructor
     *  @param  object
     *  @param  begin
     */
    TraverseIterator(zval *object, bool begin) : _object(object)
    {
        // leap out if this iterator starts at the end
        if (!begin) return;
        
        // we need the class entry
        auto *entry = zend_get_class_entry(object);
        
        // create the iterator
        _iter = entry->get_iterator(entry, object, false);
        
        // rewind the iterator
        _iter->funcs->rewind(_iter);
        
        // read the first key/value pair
        read();
    }
    
    /**
     *  Copy constructor
     *  @param  that
     */
    TraverseIterator(const TraverseIterator &that) : TraverseIterator(that._object, that._iter != nullptr)
    {
        // @todo    this is a broken implementation, the copy is at the start
        //          position, while we'd like to be at the same position
    }
    
    /**
     *  Destructor
     */
    virtual ~TraverseIterator()
    {
        // call the iterator destructor
        if (_iter) _iter->funcs->dtor(_iter);
    }

    /**
     *  Clone the object
     *  @return IteratorImpl*
     */
    virtual IteratorImpl *clone() override
    {
        return new TraverseIterator(*this);
    }

    /**
     *  Increment position (pre-increment)
     *  @return bool
     */
    virtual bool increment() override
    {
        // do we still have an iterator?
        if (!_iter) return false;
        
        // movw it forward
        _iter->funcs->move_forward(_iter);
        
        // and read current data
        read();
    }
    
    /**
     *  Decrement position (pre-decrement)
     *  @return bool
     */
    virtual bool decrement() override
    {
        // not possible with PHP iterators
        throw Exception("Impossible to iterate backwards");
    }

    /**
     *  Compare with other iterator
     *  @param  that
     *  @return bool
     */
    virtual bool equals(const IteratorImpl *that) const override
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
     *  @return bool
     */
    bool read()
    {
        // not possible when no iterator exists
        if (!_iter) return false;

        // is the iterator at a valid position?
        if (_iter->funcs->valid(_iter) == FAILURE) return invalidate();

#if PHP_VERSION_ID >= 50400

        // create a value object
        Value val;
        
        // call the function to get the key
        _iter->funcs->get_current_key(_iter, val._val);

#else

        // variable we need for fetching the key, and that will be assigned by
        // the PHP engine (this is php 5.3 code)
        char *str_key; unsigned int str_key_len; unsigned long int_key;

        // php 5.3 code, fetch the current key
        int type = _iter->funcs->get_current_key(_iter, &str_key, &str_key_len, &int_key);
        
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
        _iter->funcs->get_current_data(_iter, &zval);
        
        // wrap the zval in a value object
        _data.second = Value(*zval);
     
        // done
        return true;
    }
    
    /**
     *  Invalidate the object
     *  @return bool
     */
    bool invalidate()
    {
        // skip if already invalid
        if (!_iter) return false;
        
        // reset the iterator
        _iter->funcs->dtor(_iter);
        
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

