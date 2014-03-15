/**
 *  hashitem.h
 *
 *  HashItem is interface allows you to iterate through the various data types that support iteration.
 *
 *  @copyright 2013 Copernica BV
 */



/**
 *  Set up namespace
 */
namespace Php {


class HashItem
{
public:

    /**
     *  retrieve data value
     */
    virtual Value value() const = 0;

    /**
     *  return key
     */
    virtual Value key() const = 0;

    /**
     *  return integer key (index)
     */
    virtual unsigned long intKey() const = 0;

    /**
     *  return string key
     */
    virtual std::string strKey() const = 0;

    /**
     *  key type is string?
     */
    virtual bool isstr() const = 0;

    /**
     *  item on current position of hashtable is empty?
     */
    virtual bool isEmpty() const = 0;

    /**
     *  next iteration
     */
    virtual void next() = 0;

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() = 0;

    /**
     *  compare operator
     */
    virtual bool compare(const HashItem *rhs) const = 0;

    virtual ~HashItem() {};
};


/**
 *  End of namespace
 */
}

