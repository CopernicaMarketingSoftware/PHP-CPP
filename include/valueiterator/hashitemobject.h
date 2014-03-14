/**
 *  hashitemobject.h
 *
 *  HashItemObject
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef HASHITEMOBJECT_H
#define HASHITEMOBJECT_H

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 *  HashItemObject
 */
class HashItemObject : public HashItem
{
public:

    /**
     *  Constructor empty HashItemObject
     */
    //HashItemObject() : HashPos() {}

    /**
     *  Constructor HashItemObject
     *  @param  arr HashTable
     */
    explicit HashItemObject(_hashtable *arr) : HashPos(arr) {}

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
        // check access rights to the current item
        checkAccess();
    }

    /**
     *  previous iteration
     */
    virtual void prev() override
    {
        HashPos.prev();
    }

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() override
    {
        
        HashPos.toBegin();
        // After a reset key positions verifiable access rights to the first item
        checkAccess();
    }

    /**
     *  compare operator
     */
    //virtual bool compare(const HashItem& rhs) const override
    virtual bool compare(const HashItem *rhs) const override
    {
        return (HashPos == ((HashItemObject *)rhs)->HashPos);
    }

    virtual ~HashItemObject() {};

private:

    /**
     *  Check the permissions of property. And move on to the next property, if access is denied.
     */
    void checkAccess()
    {
        if ( !HashPos.isEmpty() && !HashPos.keyAccessible() ) next();
    }
    
    // Position in the internal hash table
    HashPositionWrapper HashPos;
};

/**
 *  End of namespace
 */
}
#endif /* hashitemobject.h */
