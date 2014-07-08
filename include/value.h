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
class Base;
class ValueIterator;
template <class Type> class HashMember;

/**
 *  Class definition
 */
class Value : private HashParent
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
    Value(long value);
    Value(bool value);
    Value(char value);
    Value(const std::string &value);
    Value(const char *value, int size = -1);
    Value(double value);
    Value(const IniValue &value);

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
    Value(const std::map<std::string,T> &value) : Value(Type::Array)
    {
        // set all elements
        for (auto &iter : value) setRaw(iter.first.c_str(), iter.first.size(), iter.second);
    }
    
    /**
     *  Wrap object around zval
     *  @param  zval        Zval to wrap
     *  @param  ref         Force this to be a reference
     *  @param  tsrm_ls     Optional pointer to thread safe data
     */
    Value(struct _zval_struct *zval, bool ref=false);
    
    /**
     *  Wrap around an object implemented by us
     *  @param  object      Object to be wrapped
     */
    Value(const Base *base);
    
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
    Value &operator=(long value);
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
    Value &operator+=(long value);
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
    Value &operator-=(long value);
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
    Value &operator*=(long value);
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
    Value &operator/=(long value);
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
    Value &operator%=(long value);
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
    Value operator+(long value);
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
    Value operator-(long value);
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
    Value operator*(long value);
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
    Value operator/(long value);
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
    Value operator%(long value);
    Value operator%(bool value);
    Value operator%(char value);
    Value operator%(const std::string &value);
    Value operator%(const char *value);
    Value operator%(double value);
    
    /**
     *  Comparison operators for hardcoded strings
     *  @param  value
     */
    bool operator==(const char *value) const { return ::strcmp(rawValue(), value) == 0; }
    bool operator!=(const char *value) const { return ::strcmp(rawValue(), value) != 0; }
    bool operator<=(const char *value) const { return ::strcmp(rawValue(), value) <= 0; }
    bool operator>=(const char *value) const { return ::strcmp(rawValue(), value) >= 0; }
    bool operator< (const char *value) const { return ::strcmp(rawValue(), value) <  0; }
    bool operator> (const char *value) const { return ::strcmp(rawValue(), value) >  0; }

    /**
     *  Comparison operators for hardcoded Value
     *  @param  value
     */
    bool operator==(const Value &value) const;
    bool operator!=(const Value &value) const { return !operator==(value); }
    bool operator< (const Value &value) const;
    bool operator> (const Value &value) const { return value.operator<(*this); }
    bool operator<=(const Value &value) const { return !operator>(value); }
    bool operator>=(const Value &value) const { return !operator<(value); }

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
    bool isList()       const;
    bool isMap()        const { return type() == Type::Array && !isList(); }
    bool isRef()        const;

    /**
     * Get a reference of this Value.
     *
     * @return Value
     */
    Value ref() const {
        return Value(_val, true);
    }

    /**
     *  Get access to the raw buffer - you can use this for direct reading and
     *  writing to and from the buffer. Note that this only works for string
     *  variables - other variables return nullptr.
     * 
     *  If you are going to write to the buffer, make sure that you first call
     *  the resize() method to ensure that the buffer is big enough.
     *  
     *  @return char *
     */
    char *buffer() const;
    
    /**
     *  Resize buffer space. If you want to write directly to the buffer (which 
     *  is returned by the buffer() method), you should first reserve enough 
     *  space in it. This can be done with this resize() method. This will also 
     *  turn the Value object into a string (if it was not already a string). 
     *  The writable buffer is returned.
     * 
     *  @param  size
     *  @return char* 
     */
    char *reserve(size_t size);
    
    /**
     *  Get access to the raw buffer for read operationrs.
     *  @return const char *
     */
    const char *rawValue() const;
    
    /**
     *  Retrieve the value as number
     *
     *  We force this to be a int64_t because we assume that most
     *  servers run 64 bits nowadays, and because we use int32_t, int64_t
     *  almost everywhere, instead of 'long' and on OSX neither of
     *  these intxx_t types is defined as 'long'...
     *
     *  @return int64_t
     */
    int64_t numericValue() const;
    
    /**
     *  Retrieve the value as boolean
     *  @return bool
     */
    bool boolValue() const;
    
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

        for (auto &iter : map) result[iter.first] = iter.second;
        
        // done
        return result;
    }

    /**
     *  Get array keys, or object property names, include private & protected properties
     *  @return std::vector
     */
    std::vector<Php::Value> keys() const;

    /**
     *  Get object property names.
     *  @param only_public
     *  @return std::vector
     */
    std::vector<std::string> properties(bool only_public = true) const;

    /**
     *  Define the iterator type
     */
    typedef ValueIterator iterator;
    
    /**
     *  Return an iterator for iterating over the values
     *  This is only meaningful for Value objects that hold an array or an object
     *  @return iterator
     */
    iterator begin() const;
    
    /**
     *  Return an iterator for iterating over the values
     *  This is only meaningful for Value objects that hold an array or an object
     *  @return iterator
     */
    iterator end() const;
    
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
    virtual bool contains(int index) const override;

    /**
     *  Is a certain key set in the array
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const std::string &key) const override
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
     *  Is a certain key set in the array
     *  @param  key
     *  @param  size
     *  @return bool
     */
    bool contains(const char *key) const
    {
        return contains(key, ::strlen(key));
    }
    
    /**
     *  Is a certain key set in the array, when that key is stored as value object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const Value &value) const override
    {
        if (value.isNumeric()) return contains(value.numericValue());
        if (value.isString()) return contains(value.rawValue(), value.size());
        return contains(value.stringValue());
    }
    
    /**
     *  Cast to a number
     *  @return int32_t
     */
    operator int16_t () const
    {
        return (int16_t)numericValue();
    }

    /**
     *  Cast to a number
     *  @return int32_t
     */
    operator int32_t () const
    {
        return (int32_t)numericValue();
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
     *  Cast to a number
     *  @return long
     */
    operator long () const
    {
        return (long)numericValue();
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
    virtual Value get(int index) const override;
    
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
    virtual Value get(const std::string &key) const override
    {
        return get(key.c_str(), key.size());
    }
    
    /**
     *  Get access to a certain variant member
     *  @param  key
     *  @return Value
     */
    virtual Value get(const Value &key) const override
    {
        if (key.isNumeric()) return get(key.numericValue());
        if (key.isString()) return get(key.rawValue(), key.size());
        return get(key.stringValue());
    }
    
    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  index       Index of the property to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    virtual void set(int index, const Value &value) override;
    
    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  key         Key of the property to set
     *  @param  size        Size of the key
     *  @param  value       Value to set
     */
    virtual void set(const char *key, int size, const Value &value);

    /**
     *  Set a certain property
     *  Calling this method will turn the object into an array
     *  @param  key         Key to set
     *  @param  value       Value to set
     */
    void set(const char *key, const Value &value)
    {
        set(key, ::strlen(key), value);
    }
    
    /**
     *  Set a certain property
     *  Calling this method will turn the object into an array
     *  @param  key         Key to set
     *  @param  value       Value to set
     */
    virtual void set(const std::string &key, const Value &value) override
    {
        return set(key.c_str(), key.size(), value);
    }
    
    /**
     *  Overwrite the value at a certain variant index
     *  @param  key
     *  @param  value
     */
    virtual void set(const Value &key, const Value &value) override
    {
        if (key.isNumeric()) return set(key.numericValue(), value);
        if (key.isString()) return set(key.rawValue(), key.size(), value);
        return set(key.stringValue(), value);
    }

    /**
     *  Unset a member by its index
     *  @param  index
     */
    virtual void unset(int index) override;
    
    /**
     *  Unset by key name and length of the key
     *  @param  key
     *  @param  size
     */
    void unset(const char *key, int size);
    
    /**
     *  Unset by key name and length of the key
     *  @param  key
     *  @param  size
     */
    void unset(const char *key)
    {
        unset(key, ::strlen(key));
    }

    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const std::string &key) override
    {
        return unset(key.c_str(), key.size());
    }

    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const Value &key) override
    {
        if (key.isNumeric()) return unset(key.numericValue());
        if (key.isString()) return unset(key.rawValue(), key.size());
        return unset(key.stringValue());
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
     *  Index by other value object
     *  @param  key
     *  @return HashMember<std::string>
     */
    HashMember<Value> operator[](const Value &key);

    /**
     *  Index by other value object
     *  @param  key
     *  @return HashMember<std::string>
     */
    Value operator[](const Value &key) const
    {
        if (key.isNumeric()) return get(key.numericValue());
        if (key.isString()) return get(key.rawValue(), key.size());
        return get(key.stringValue());
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

    /**
     *  To check for reference equality.
     *  @param  value
     *  @return bool
     */
    bool refequals(const Value &value) const
    {
        return isRef() && _val == value._val;
    }

    /**
     *  Return a hash value for unordered_map.
     *  @return size_t
     */
    size_t hash() const;

    /**
     *  Return the class name of object.
     *  Return empty string when this value is not a object.
     *  @return std::string
     */
    std::string className() const;

    /**
     *  Return a id of object. (It is like spl_object_hash)
     *  @return std::string
     */
    std::string id() const;

    /**
     *  Checks if this object is of the class or has the class as one of its parents
     *  @param classname
     *  @param allow_string
     *  @return bool
     */
    inline bool is(const std::string &classname, bool allow_string=false) const {
        return isImpl(classname, allow_string, false);
    }

    /**
     *  Checks if this object has the class as one of its parents
     *  @param classname
     *  @return bool
     */
    inline bool isSubClassOf(const std::string &classname, bool allow_string=true) const {
        return isImpl(classname, allow_string, true);
    }

private:

    bool isImpl(const std::string &classname, bool allow_string, bool only_subclass) const;

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

protected:
    /**
     *  The wrapped zval
     *  @var struct zval
     */
    struct _zval_struct *_val;
    
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
     *  Attach a different zval
     * 
     *  This will first detach the current zval, and link the Value object to 
     *  a different zval. Versions exist to attach to a zval and to an entire
     *  hash table
     * 
     *  @param  val
     */
    void attach(struct _zval_struct *val);
    void attach(struct _hashtable *hashtable);
    
    /**
     *  Set a certain property without running any checks (you must already know
     *  for sure that this is an array, and that the index is not yet in use)
     * 
     *  @param  index       Index of the property to set
     *  @param  value       Value to set
     */
    void setRaw(int index, const Value &value);
    
    /**
     *  Set a certain property without any checks (you must already know for
     *  sure that this is either an object or an array, and that the index is
     *  not yet in use)
     * 
     *  @param  key         Key of the property to set
     *  @param  size        Size of the key
     *  @param  value       Value to set
     */
    void setRaw(const char *key, int size, const Value &value);

    /**
     *  Internal helper method to create an iterator
     *  @param  begin       Should the iterator start at the begin?
     *  @return iterator
     */
    iterator createIterator(bool begin) const;
    
    /**
     *  The Globals and Member classes can access the zval directly
     */
    friend class Globals;
    friend class Member;
    friend class ClassImpl;
    friend class IteratorImpl;
    friend class Extension;
    friend class HashIterator;
    friend class TraverseIterator;
    friend class HashMember<int>;
    friend class HashMember<std::string>;
    friend class Callable;
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


