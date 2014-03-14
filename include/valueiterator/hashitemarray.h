/**
 *  hashitemarray.h
 *
 *  HashItemArray
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef HASHITEMARRAY_H
#define HASHITEMARRAY_H

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 *  HashItemArray
 */
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
     *  return integer key (index)
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
     *  is hashtable item is empty?
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
#endif /* hashitemarray.h */
