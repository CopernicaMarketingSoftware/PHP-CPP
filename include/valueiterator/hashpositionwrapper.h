/**
 *  hashpositionwrapper.h
 *
 *  HashPositionWrapper


  is a simple type that is used only in conjunction with ValueIterator.
 *  Intended for obtaining access to the keys, values, and types of key variable of type Value during iteration
 *
 *  You are not supposed to instantiate this class.
 *  An instance of this class is created automatically when you create an object of class Value::iterator
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef HTPWRAPPER_H
#define HTPWRAPPER_H

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

/**
 *  Class definition
 *  HashPositionWrapper
 */
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
    //const char* key() const;
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
    bool isEmpty() const {
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

    // get access to the hast table (class HashTable)
    _hashtable *ht;
};

/**
 *  End of namespace
 */
}
#endif /* hashpositionwrapper.h */
