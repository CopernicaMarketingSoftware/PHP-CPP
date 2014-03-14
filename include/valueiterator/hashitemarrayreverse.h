/**
 *  hashitemarrayreverse.h
 *
 *  HashItemArrayReverse
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef HASHITEMARRAYREVERSE_H
#define HASHITEMARRAYREVERSE_H

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 *  HashItemArrayReverse
 */
class HashItemArrayReverse : public HashItemArray
{
public:

    /**
     *  Constructor HashItemArrayReverse
     *  @param  arr HashTable
     */
    explicit HashItemArrayReverse(_hashtable *arr) : HashItemArray(arr) {}

    /**
     *  next iteration
     */
    virtual void next() override
    {
        HashPos.prev();
    }

    /**
     *  reset iterator to beginning of the hash table
     */
    virtual void reset() override
    {
        HashPos.toEnd();
    }

    /**
     *  compare operator
     */
    virtual bool compare(const HashItem *rhs) const override
    {
        return (HashPos == ((HashItemArrayReverse *)rhs)->HashPos);
    }

    virtual ~HashItemArrayReverse() {};
    
};

/**
 *  End of namespace
 */
}
#endif /* HashItemArrayReverse.h */
