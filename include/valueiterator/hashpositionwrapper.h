/**
 *  hashpositionwrapper.h
 *
 *  HashPositionWrapper - is wrapper over Zend for easy management of position in the internal hash table
 *
 *  @copyright 2013 Copernica BV
 */


/**
 *  Forward declaration
 */
struct _hashtable;
struct bucket;
typedef bucket* _HashPosition;

/**
 *  Set up namespace
 */
namespace Php {


class HashPositionWrapper
{
public:

    /**
     *  Constructor empty HashPositionWrapper
     */
    HashPositionWrapper() : ht(nullptr), pos(nullptr) {}

    /**
     *  Constructor HashPositionWrapper
     *  @param  arr HashTable
     */
    explicit HashPositionWrapper(_hashtable *arr);

    /**
     *  retrieve data value
     */
    Value value() const;

    /**
     *  return string key
     */
    std::string key() const;

    /**
     *  return integer key (index)
     */
    unsigned long ind() const;

    /**
     *  key type is string?
     */
    bool isstr() const;

    /**
     *  Check the permissions of key.
     *  It is used when the iteration of object.
     */
    bool keyAccessible() const;

    /**
     *  is hashtable item is empty?
     */
    bool isEmpty() const
    {
        return !pos;
    }

    /**
     *  next iteration
     */
    void next();

    /**
     *  previous iteration
     */
    void prev();

    /**
     *  reset iterator to beginning of the hash table
     */
    void toBegin();

    /**
     *  reset iterator to end of the hash table
     */
    void toEnd();

    /**
     *  compare operator
     */
    bool operator==(const HashPositionWrapper& rhs) const;



private:
    
    // Position in the internal hash table
    _HashPosition pos;

    // get access to the hast table (struct HashTable)
    _hashtable *ht;
};

/**
 *  End of namespace
 */
}

