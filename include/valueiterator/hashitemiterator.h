/**
 *  hashitemiterator.h
 *
 *
 *  HashItemIterator - allows iterate objects of a class that implements the PHP Iterator interface
 *
 *
 *  @copyright 2013 Copernica BV
 */



/**
 *  Forward declaration
 */
struct _zend_object_iterator;
struct _zend_object_iterator_funcs;

/**
 *  Set up namespace
 */
namespace Php {


class HashItemIterator : public HashItem
{
public:

    /**
     *  Constructor HashItemIterator
     *  @param  arr HashTable
     */
    HashItemIterator(_zend_class_entry* ce, _zval_struct* val);

    /**
     *  retrieve data value
     */
    virtual Value value() const override;

    /**
     *  return key
     */
    virtual Value key() const override;

    /**
     *  return integer key (index)
     */
    virtual unsigned long intKey() const override;

    /**
     *  return string key
     */
    virtual std::string strKey() const override;

    /**
     *  key type is string?
     */
    virtual bool isstr() const override;

    /**
     *  is hashtable item is empty?
     */
    virtual bool isEmpty() const override;

    /**
     *  next iteration
     */
    virtual void next() override;

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() override;

    /**
     *  compare operator
     */
    virtual bool compare(const HashItem *rhs) const override;

    virtual ~HashItemIterator();

private:

	// Internal iterator of iterable object
	_zend_object_iterator *iter;

	_zend_object_iterator_funcs *funcs;

};

/**
 *  End of namespace
 */
}
