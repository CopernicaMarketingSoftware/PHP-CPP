/**
 *  hashitemobjectreverse.h
 *
 *  HashItemObjectReverse
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef HASHITEMOBJECTREVERSE_H
#define HASHITEMOBJECTREVERSE_H

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 *  HashItemObjectReverse
 */
class HashItemObjectReverse : public HashItemArray
{
public:

    /**
     *  Constructor HashItemObjectReverse
     *  @param  arr HashTable
     */
     explicit HashItemObjectReverse(_hashtable *arr) : HashItemArray(arr) {}

    /**
     *  next iteration
     */
    virtual void next() override
    {
        HashPos.prev();
        // check access rights to the current item
        checkAccess();
    }

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() override
    {
        HashPos.toEnd();
        // After a reset key positions verifiable access rights to the first item
        checkAccess();
    }

    /**
     *  compare operator
     */
    virtual bool compare(const HashItem *rhs) const override
    {
        return (HashPos == ((HashItemObjectReverse *)rhs)->HashPos);
    }

    virtual ~HashItemObjectReverse() {};

private:

    /**
     *  Check the permissions of property. And move on to the next property, if access is denied.
     */
    void checkAccess()
    {
        if ( !HashPos.isEmpty() && !HashPos.keyAccessible() ) next();
    }
    
};

/**
 *  End of namespace
 */
}
#endif /* HashItemObjectReverse.h */
