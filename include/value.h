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
template <class Type> class HashMember;

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
     *  Constructor for various types
     *  @param  value
     */
    Value(std::nullptr_t value);
    Value(int16_t value);
    Value(int32_t value);
    Value(int64_t value);
    Value(bool value);
    Value(char value);
    Value(const std::string &value);
    Value(const char *value, int size = -1);
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
     *  Move assignment
     *  @param  value
     *  @return Value
     */
    Value &operator=(Value &&value);
    
    /**
     *  Assignment operator for various types
     *  @param  value
     *  @return Value
     */
    Value &operator=(const Value &value);
    Value &operator=(int16_t value);
    Value &operator=(int32_t value);
    Value &operator=(int64_t value);
    Value &operator=(bool value);
    Value &operator=(char value);
    Value &operator=(const std::string &value);
    Value &operator=(const char *value);
    Value &operator=(double value);
    
    /**
     *  Add a value to the object
     *  @param  value
     *  @return Value
     */
    Value &operator+=(const Value &value);
    Value &operator+=(int16_t value);
    Value &operator+=(int32_t value);
    Value &operator+=(int64_t value);
    Value &operator+=(bool value);
    Value &operator+=(char value);
    Value &operator+=(const std::string &value);
    Value &operator+=(const char *value);
    Value &operator+=(double value);

    /**
     *  Subtract a value from the object
     *  @param  value
     *  @return Value
     */
    Value &operator-=(const Value &value);
    Value &operator-=(int16_t value);
    Value &operator-=(int32_t value);
    Value &operator-=(int64_t value);
    Value &operator-=(bool value);
    Value &operator-=(char value);
    Value &operator-=(const std::string &value);
    Value &operator-=(const char *value);
    Value &operator-=(double value);
    
    /**
     *  Multiply the object with a certain value
     *  @param  value
     *  @return Value
     */
    Value &operator*=(const Value &value);
    Value &operator*=(int16_t value);
    Value &operator*=(int32_t value);
    Value &operator*=(int64_t value);
    Value &operator*=(bool value);
    Value &operator*=(char value);
    Value &operator*=(const std::string &value);
    Value &operator*=(const char *value);
    Value &operator*=(double value);

    /**
     *  Divide the object with a certain value
     *  @param  value
     *  @return Value
     */
    Value &operator/=(const Value &value);
    Value &operator/=(int16_t value);
    Value &operator/=(int32_t value);
    Value &operator/=(int64_t value);
    Value &operator/=(bool value);
    Value &operator/=(char value);
    Value &operator/=(const std::string &value);
    Value &operator/=(const char *value);
    Value &operator/=(double value);

    /**
     *  Divide the object with a certain value, and get the rest
     *  @param  value
     *  @return Value
     */
    Value &operator%=(const Value &value);
    Value &operator%=(int16_t value);
    Value &operator%=(int32_t value);
    Value &operator%=(int64_t value);
    Value &operator%=(bool value);
    Value &operator%=(char value);
    Value &operator%=(const std::string &value);
    Value &operator%=(const char *value);
    Value &operator%=(double value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value operator+(const Value &value);
    Value operator+(int16_t value);
    Value operator+(int32_t value);
    Value operator+(int64_t value);
    Value operator+(bool value);
    Value operator+(char value);
    Value operator+(const std::string &value);
    Value operator+(const char *value);
    Value operator+(double value);

    /**
     *  Subtraction operator
     *  @param  value
     *  @return Value
     */
    Value operator-(const Value &value);
    Value operator-(int16_t value);
    Value operator-(int32_t value);
    Value operator-(int64_t value);
    Value operator-(bool value);
    Value operator-(char value);
    Value operator-(const std::string &value);
    Value operator-(const char *value);
    Value operator-(double value);

    /**
     *  Multiplication operator
     *  @param  value
     *  @return Value
     */
    Value operator*(const Value &value);
    Value operator*(int16_t value);
    Value operator*(int32_t value);
    Value operator*(int64_t value);
    Value operator*(bool value);
    Value operator*(char value);
    Value operator*(const std::string &value);
    Value operator*(const char *value);
    Value operator*(double value);

    /**
     *  Division operator
     *  @param  value
     *  @return Value
     */
    Value operator/(const Value &value);
    Value operator/(int16_t value);
    Value operator/(int32_t value);
    Value operator/(int64_t value);
    Value operator/(bool value);
    Value operator/(char value);
    Value operator/(const std::string &value);
    Value operator/(const char *value);
    Value operator/(double value);

    /**
     *  Modulus operator
     *  @param  value
     *  @return Value
     */
    Value operator%(const Value &value);
    Value operator%(int16_t value);
    Value operator%(int32_t value);
    Value operator%(int64_t value);
    Value operator%(bool value);
    Value operator%(char value);
    Value operator%(const std::string &value);
    Value operator%(const char *value);
    Value operator%(double value);
    
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
     *  Check if the value is of a certain type
     *  @return bool
     */
    bool isNull()       const { return type() == nullType; }
    bool isNumeric()    const { return type() == numericType; }
    bool isBool()       const { return type() == boolType; }
    bool isString()     const { return type() == stringType; }
    bool isFloat()      const { return type() == floatType; }
    bool isObject()     const { return type() == objectType; }
    bool isArray()      const { return type() == arrayType; }
    bool isCallable() 	const { return type() == callableType; }
    
    /**
     *  Retrieve the value as number
     *  @return long
     */
    long numericValue() const;
    
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
    double floatValue() const;

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
     *  Cast to a number
     *  @return int32_t
     */
    operator int16_t () const
    {
        return numericValue();
    }

    /**
     *  Cast to a number
     *  @return int32_t
     */
    operator int32_t () const
    {
        return numericValue();
    }

    /**
     *  Cast to a number
     *  @return uint64_t
     */
    operator int64_t () const
    {
        return numericValue();
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
        return floatValue();
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
    HashMember<int> operator[](int index);
    
    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    HashMember<std::string> operator[](const std::string &key);

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember<std::string> operator[](const char *key);

    Value call(std::initializer_list<Value> params);

protected:
    /**
     *  The wrapped zval
     *  @var struct zval
     */
    struct _zval_struct *_val;

    /**
     *  The environment can access the zval directly
     */
    friend class Environment;
    friend class Member;
    friend class ObjectInfo;
};

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const Value &value);

/**
 *  End of namespace
 */
}


