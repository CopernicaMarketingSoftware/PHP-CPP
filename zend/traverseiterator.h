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
     */
    TraverseIterator(zval *object, bool begin) : _object(object)
    {
        // leap out if this iterator starts at the end
        if (!begin) return;

        // we need the class entry
        auto *entry = Z_OBJCE_P(object);

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
        // do nothing if iterator is already invalid
        if (!_iter) return;

        // call the iterator destructor
        if (_iter) _iter->funcs->dtor(_iter);
    }

    /**
     *  Clone the object
     *  @return ValueIteratorImpl*
     */
    virtual ValueIteratorImpl *clone() override
    {
        // construct iterator
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

        // move it forward
        _iter->funcs->move_forward(_iter);

        // and read current data
        read();

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
    virtual const std::pair<Value,Value> &current() const override
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

        // create a value object
        Value val;

        // call the function to get the key
        _iter->funcs->get_current_key(_iter, val._val);

        // store the key
        _data.first = val;

        // get the current value
        auto *zval = _iter->funcs->get_current_data(_iter);

        // wrap the zval in a value object
        _data.second = Value(zval);

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

