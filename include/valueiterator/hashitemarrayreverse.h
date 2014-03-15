/**
 *  hashitemarrayreverse.h
 *
 *  
 *  HashItemArrayReverse - Allows reverse iteration of regular PHP-array
 *
 *
 *  @copyright 2013 Copernica BV
 */



/**
 *  Set up namespace
 */
namespace Php {


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
