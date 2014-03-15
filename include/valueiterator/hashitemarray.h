/**
 *  hashitemarray.h
 *
 *  
 *  HashItemArray - Allows iteration of regular PHP-array
 *
 *
 *  @copyright 2013 Copernica BV
 */



/**
 *  Set up namespace
 */
namespace Php {


class HashItemArray : public HashItem
{
public:

    /**
     *  Constructor HashItemArray
     *  @param  arr HashTable
     */
    explicit HashItemArray(_hashtable *arr) : HashPos(arr) {}

    /**
     *  retrieve data value
     */
    virtual Value value() const override
    {
        return HashPos.value();
    }

    /**
     *  return key
     */
    virtual Value key() const override
    {
        return HashPos.isstr() ? Value(HashPos.key()) : Value((signed long int)HashPos.ind());
    }

    /**
     *  return integer key (index)
     */
    virtual unsigned long intKey() const override
    {
        return HashPos.ind();
    }

    /**
     *  return string key
     */
    virtual std::string strKey() const override
    {
        return HashPos.key();
    }

    /**
     *  key type is string?
     */
    virtual bool isstr() const override
    {
        return HashPos.isstr();
    }

    /**
     *  current item of hashtable  is empty?
     */
    virtual bool isEmpty() const override
    {
        return HashPos.isEmpty();
    }

    /**
     *  next iteration
     */
    virtual void next() override
    {
        HashPos.next();
    }

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() override
    {
        HashPos.toBegin();
    }

    /**
     *  compare operator
     */
    virtual bool compare(const HashItem *rhs) const override
    {
        return (HashPos == ((HashItemArray *)rhs)->HashPos);
    }

    virtual ~HashItemArray() {};

protected:
    
    // Position in the internal hash table
    HashPositionWrapper HashPos;
};

/**
 *  End of namespace
 */
}

