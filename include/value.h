/**
 *  Value.h
 *
 *  Base class for values that are stored in the Zend engine. One instance
 *  of the value class represents a variable that exists in user space in
 *  the PHP environment, for example as global variable, local variable
 *  inside a function or as a member of an object or an array.
 * 
 *  A value can be a scalar or a more complicated structure like an object
 *  or an array.
 * 
 *  Internally, the Zend engine works with "zval" objects for this. These "zval"
 *  object hold a reference counter and a reference setting. The PHP-CPP Value
 *  class takes care of doing this, so all you need to do is use objects of
 *  this class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward definitions
 */
struct _zval_struct;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward definitions
 */
template <class Type> class Member;

/**
 *  Class definition
 */
class Value
{
public:
    /**
     *  Empty constructor (value = NULL)
     */
    Value();

    /**
     *  Constructor for null ptr
     */
    Value(std::nullptr_t value);

    /**
     *  Constructor based on integer value
     *  @param  value
     */
    Value(int value);
    
    /**
     *  Constructor based on integer value
     *  @param  value
     */
    Value(long value);
    
    /**
     *  Constructor based on boolean value
     *  @param  value
     */
    Value(bool value);
    
    /**
     *  Constructor based on single character
     *  @param  value
     */
    Value(char value);
    
    /**
     *  Constructor based on string value
     *  @param  value
     */
    Value(const std::string &value);
    
    /**
     *  Constructor based on byte buffer
     *  @param  value
     *  @param  size
     */
    Value(const char *value, int size = -1);
    
    /**
     *  Constructor based on decimal value
     *  @param  value
     */
    Value(double value);
    
    /**
     *  Wrap object around zval
     *  @param  zval        Zval to wrap
     *  @param  ref         Force this to be a reference
     */
    Value(struct _zval_struct *zval, bool ref = false);
    
    /**
     *  Copy constructor
     *  @param  value
     */
    Value(const Value &that);
    
    /**
     *  Move constructor
     *  @param  value
     */
    Value(Value &&that);
    
    /**
     *  Destructor
     */
    virtual ~Value();

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(const Value &value);
    
    /**
     *  Move assignment
     *  @param  value
     *  @return Value
     */
    Value &operator=(Value &&value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(long value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(int value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(bool value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(char value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(const std::string &value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(const char *value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(double value);
    
    /**
     *  The type of object
     *  @return Type
     */
    Type type() const;
    
    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    Value &setType(Type type);

    /**
     *  Make a clone of the value with the same type
     *  @return Value
     */
    Value clone() const;

    /**
     *  Make a clone of the value with a different type
     *  @param  type
     *  @return Value
     */
    Value clone(Type type) const;

    /**
     *  Is this a NULL value?
     *  @return bool
     */
    bool isNull() const
    {
        return type() == nullType;
    }

    /**
     *  Is this an integer value?
     *  @return bool
     */
    bool isLong() const
    {
        return type() == longType;
    }
    
    /**
     *  Is this a boolean value?
     *  @return bool
     */
    bool isBool() const
    {
        return type() == boolType;
    }
    
    /**
     *  Is this a string value?
     *  @return bool
     */
    bool isString() const
    {
        return type() == stringType;
    }
    
    /**
     *  Is this a decimal value?
     *  @return bool
     */
    bool isDecimal() const
    {
        return type() == decimalType;
    }
    
    /**
     *  Is this an object value?
     *  @return bool
     */
    bool isObject() const
    {
        return type() == objectType;
    }
    
    /**
     *  Is this an array value?
     *  @return bool
     */
    bool isArray() const
    {
        return type() == arrayType;
    }
    
    /**
     *  Retrieve the value as integer
     *  @return int
     */
    long longValue() const;
    
    /**
     *  Retrieve the value as boolean
     *  @return bool
     */
    bool boolValue() const;
    
    /**
     *  Retrieve the raw string value
     *  Warning: Only use this for NULL terminated strings, or use it in combination 
     *  with the string size to prevent that you access data outside the buffer
     *  @return const char *
     */
    const char *rawValue() const;
    
    /**
     *  Retrieve the value as a string
     *  @return string
     */
    std::string stringValue() const;
    
    /**
     *  Retrieve the value as decimal
     *  @return double
     */
    double decimalValue() const;
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int size() const;
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int count() const
    {
        return size();
    }
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int length() const
    {
        return size();
    }
    
    /**
     *  Is a certain index set in the array
     *  @param  index
     *  @return bool
     */
    bool contains(int index) const;

    /**
     *  Is a certain key set in the array
     *  @param  key
     *  @return bool
     */
    bool contains(const std::string &key) const
    {
        return contains(key.c_str(), key.size());
    }
    
    /**
     *  Is a certain key set in the array
     *  @param  key
     *  @param  size
     *  @return bool
     */
    bool contains(const char *key, int size) const;
    
    /**
     *  Cast to a long
     *  @return long
     */
    operator long () const
    {
        return longValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
        return boolValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return stringValue();
    }
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
        return rawValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double () const
    {
        return decimalValue();
    }
    
    /**
     *  Get access to a certain array member
     *  @param  index
     *  @return Value
     */
    Value get(int index) const;
    
    /**
     *  Get access to a certain assoc member
     *  @param  key
     *  @param  size
     *  @return Value
     */
    Value get(const char *key, int size=-1) const;
    
    /**
     *  Get access to a certain assoc member
     *  @param  key
     *  @return Value
     */
    Value get(const std::string &key) const
    {
        return get(key.c_str(), key.size());
    }
    
    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  index       Index of the property to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    virtual const Value &set(int index, const Value &value);
    
    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  key         Key of the property to set
     *  @param  size        Size of the key
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    virtual const Value &set(const char *key, int size, const Value &value);

    /**
     *  Set a certain property
     *  Calling this method will turn the object into an array
     *  @param  key         Key to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    const Value &set(const char *key, const Value &value)
    {
        return set(key, strlen(key), value);
    }
    
    /**
     *  Set a certain property
     *  Calling this method will turn the object into an array
     *  @param  key         Key to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    const Value &set(const std::string &key, const Value &value)
    {
        return set(key.c_str(), key.size(), value);
    }
    
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  @param  index
     *  @return Member
     */
    Member<int> operator[](int index);
    
    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    Member<std::string> operator[](const std::string &key);

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    Member<std::string> operator[](const char *key);


protected:
    /**
     *  The wrapped zval
     *  @var struct zval
     */
    struct _zval_struct *_val;
};

/**
 *  End of namespace
 */
}


