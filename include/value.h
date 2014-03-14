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
 *  Forward declaration
 */
class ValueIterator;
class HashItem;

/**
 *  Forward definitions
 */
class Base;
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
     *  Construct to a specific type
     *  @param  value
     */
    Value(Type type) : Value() { setType(type); }
    
    /**
     *  Constructors from a vector (this will create an array)
     *  @param  value
     */
    template <typename T>
    Value(const std::vector<T> &input) : Value(Type::Array)
    {
        // index
        int i = 0;
        
        // set all elements
        for (auto &elem : input) setRaw(i++, elem);
    }

    /**
     *  Constructor from an initializer list
     *  @param  value
     */
    template <typename T>
    Value(const std::initializer_list<T> &value) : Value(Type::Array) 
    {
        // index
        int i = 0;

        // set all elements
        for (auto &elem : value) setRaw(i++, elem);
    }
    
    /**
     *  Constructor from a map (this will create an associative array)
     *  @param  value
     */
    template <typename T>
    Value(const std::map<std::string,T> &value)
    {
        // set all elements
        for (auto &iter : value) setRaw(iter.first.c_str(), iter.first.size(), iter.second);
    }
    
    /**
     *  Wrap object around zval
     *  @param  zval        Zval to wrap
     *  @param  ref         Force this to be a reference
     */
    Value(struct _zval_struct *zval, bool ref = false);
    
    /**
     *  Wrap around an object implemented by us
     *  @param  object      Object to be wrapped
     */
    Value(Base *base);
    
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
    Value &operator=(std::nullptr_t value);
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
     *  Comparison operators
     *  @param  value
     */
    template <typename T> bool operator==(const T &value) const { return (T)*this == value; }
    template <typename T> bool operator!=(const T &value) const { return (T)*this != value; }
    template <typename T> bool operator<=(const T &value) const { return (T)*this <= value; }
    template <typename T> bool operator>=(const T &value) const { return (T)*this >= value; }
    template <typename T> bool operator< (const T &value) const { return (T)*this <  value; }
    template <typename T> bool operator> (const T &value) const { return (T)*this >  value; }

    /**
     *  The type of object
     *  @return Type
     */
    Type type() const;
    
    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    virtual Value &setType(Type type);

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
    bool isNull()       const { return type() == Type::Null; }
    bool isNumeric()    const { return type() == Type::Numeric; }
    bool isBool()       const { return type() == Type::Bool; }
    bool isString()     const { return type() == Type::String; }
    bool isFloat()      const { return type() == Type::Float; }
    bool isObject()     const { return type() == Type::Object; }
    bool isArray()      const { return type() == Type::Array; }
    bool isCallable()   const;
    
    /**
     *  Is the variable empty?
     *  @return bool
     */
    bool isEmpty() const;
    
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
     *  Convert the object to a vector
     * 
     *  This only works for regular arrays that are indexed by a number, start
     *  with position 0 and have no empty spaces.
     *  
     *  @return std::vector
     */
    template <typename T>
    std::vector<T> vectorValue() const
    {
        // only works for arrays, other types return an empty vector
        if (!isArray()) return std::vector<T>();

        // allocate a result
        std::vector<T> result;
        
        // reserve enough space
        size_t count = size();
        result.reserve(count);
        
        // and fill the result vector
        for (size_t i = 0; i<count; i++) 
        {
            // check if the index exists
            if (!contains(i)) continue;
            
            // get the value object
            Value value(get(i));
            
            // add it to the vector
            result.push_back(value);
        }
        
        // done
        return result;
    }
    
    /**
     *  Convert the object to a map with string index and Php::Value value
     *  @return std::map
     */
    std::map<std::string,Php::Value> mapValue() const;
    
    /**
     *  Convert the object to a map with string index and a specific type as value
     *  @return std::map
     */
    template <typename T>
    std::map<std::string,T> mapValue() const
    {
        // must be an array or an object, otherwise the map is empty
        if (!isArray() && !isObject()) return std::map<std::string,T>();
        
        // get the original map value
        std::map<std::string,Php::Value> map(mapValue());
        
        // result variable
        std::map<std::string,T> result;
        
        // done
        return result;
    }
    
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
     *  Convert the object to a vector
     *  @return std::vector
     */
    template <typename T>
    operator std::vector<T>() const
    {
        return vectorValue<T>();
    }

    /**
     *  Convert the object to a map with string index and Php::Value value
     *  @return std::map
     */
    operator std::map<std::string,Php::Value> () const
    {
        return mapValue();
    }

    /**
     *  Convert the object to a map with string index and Php::Value value
     *  @return std::map
     */
    template <typename T>
    operator std::map<std::string,T> () const
    {
        return mapValue<T>();
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
     *  This can be used for accessing arrays
     *  @param  index
     *  @return Value
     */
    Value operator[](int index) const
    {
        return get(index);
    }
    
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
     *  @return Value
     */
    Value operator[](const std::string &key) const
    {
        return get(key);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember<std::string> operator[](const char *key);

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Value
     */
    Value operator[](const char *key) const
    {
        return get(key);
    }

    /**
     *  Call the function in PHP
     *  We have ten variants of this function, depending on the number of parameters
     *  This call operator is only useful when the variable represents a callable
     *  @return Value
     */
    Value operator()() const;
    Value operator()(Value p0) const;
    Value operator()(Value p0, Value p1) const;
    Value operator()(Value p0, Value p1, Value p2) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4, Value p5) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8) const;
    Value operator()(Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8, Value p9) const;

    /**
     *  Call a method
     *  We have ten variants of this function, depending on the number of parameters
     *  This is only applicable when the Value contains PHP object
     *  @param  name        Name of the function
     *  @return Value
     */
    Value call(const char *name);
    Value call(const char *name, Value p0);
    Value call(const char *name, Value p0, Value p1);
    Value call(const char *name, Value p0, Value p1, Value p2);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8);
    Value call(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8, Value p9);

    /**
     *  Retrieve the original implementation
     * 
     *  This only works for classes that were implemented using PHP-CPP,
     *  it returns nullptr for all other classes
     * 
     *  @return Base*
     */
    Base *implementation() const;

    /**
     *  Retrieve the original implementation
     * 
     *  This only works for classes that were implemented using PHP-CPP,
     *  it returns nullptr for all other classes
     * 
     *  @return mixed
     */
    template <typename T>
    T *implementation() const
    {
        // retrieve the implementation
        Base *base = implementation();
        if (!base) return nullptr;
        
        // try casting it
        return dynamic_cast<T*>(base);
    }

private:
    /**
     *  Call function with a number of parameters
     *  @param  argc        Number of parameters
     *  @param  argv        The parameters
     *  @return Value
     */
    Value exec(int argc, struct _zval_struct ***params) const;

    /**
     *  Call method with a number of parameters
     *  @param  name        Name of method to call
     *  @param  argc        Number of parameters
     *  @param  argv        The parameters
     *  @return Value
     */
    Value exec(const char *name, int argc, struct _zval_struct ***params);

    /**
     *  Refcount - the number of references to the value
     *  @return int
     */
    int refcount();

public:    
    typedef ValueIterator iterator;
    
    /**
     *  Iterator to beginning
     *  @return ValueIterator&
     */
    //iterator& begin();
     iterator begin();

    /**
     *  Iterator to end
     *  @return ValueIterator&
     */
    //iterator& end() const;
    iterator end() const;


protected:
    /**
     *  The wrapped zval
     *  @var struct zval
     */
    struct _zval_struct *_val;

    /**
     *  ValueIterator pointer
     *  @var ValueIterator*
     */
    //iterator *_iterator = nullptr;
     HashItem *_hashitem = nullptr;
    
    /**
     *  Detach the zval
     * 
     *  This will unlink the zval internal structure from the Value object,
     *  so that the destructor will not reduce the number of references and/or
     *  deallocate the zval structure. This is used for functions that have to
     *  return a zval pointer, that would otherwise be deallocated the moment
     *  the function returns.
     * 
     *  @return zval
     */
    struct _zval_struct *detach();
    
    /**
     *  Set a certain property without running any checks (you must already know
     *  for sure that this is an array, and that the index is not yet in use)
     * 
     *  @param  index       Index of the property to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    const Value &setRaw(int index, const Value &value);
    
    /**
     *  Set a certain property without any checks (you must already know for
     *  sure that this is either an object or an array, and that the index is
     *  not yet in use)
     * 
     *  @param  key         Key of the property to set
     *  @param  size        Size of the key
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    const Value &setRaw(const char *key, int size, const Value &value);
    
    /**
     *  The Globals and Member classes can access the zval directly
     */
    friend class Globals;
    friend class Member;
    friend class ClassBase;
    friend class Iterator;
};

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const Value &value);

/**
 *  Custom +=, -=, *=, /=, &= operators, to update integral types with a Php::Value
 * 
 *  This code looks complicated, it ensures that the operators are only
 *  overloaded for integral types (int, bool, etc) - and not for complex types
 *  (arrays, objects, etc)
 */
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type* = nullptr> 
X &operator+=(X &x, const Php::Value &value) { return x += (X)value; }
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type* = nullptr> 
X &operator-=(X &x, const Php::Value &value) { return x -= (X)value; }
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type* = nullptr> 
X &operator*=(X &x, const Php::Value &value) { return x *= (X)value; }
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type* = nullptr> 
X &operator/=(X &x, const Php::Value &value) { return x /= (X)value; }
template <typename X, typename std::enable_if<std::is_integral<X>::value>::type* = nullptr> 
X &operator%=(X &x, const Php::Value &value) { return x %= (X)value; }

/**
 *  End of namespace
 */
}


