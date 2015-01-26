/**
 *  HashMember.h
 *
 *  When you're accessing members in an array or an object, you're
 *  doing this via an internal member object. This is an object that
 *  keeps track of the array to which it belongs, and that will update
 *  the array when the member is modified
 *
 *  You are not supposed to instantiate this class. An instance of it is 
 *  created when you call Value::operator[]
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {
    
/**
 *  Forward definitions
 */
class Value;

/**
 *  Member class
 */
template <typename Type>
class HashMember : private HashParent
{
public:
    /**
     *  Constructor
     *  @param  parent
     *  @param  index
     */
    HashMember(HashParent *parent, Type index) : _parent(parent), _index(index) {}
    
    /**
     *  Destructor
     */
    virtual ~HashMember() {}

    /**
     *  Assign a value object to the array
     *  @param  value
     *  @return Member
     */
    HashMember &operator=(const Value &value)
    {
        // set new value in the parent
        _parent->set(_index, value);

        // done
        return *this;
    }

    /**
     *  Is this an existing hash member (true) or only one that is ready
     *  to be assigned a new value to, but that is not yet in the hashtable
     *  @return bool
     */
    bool exists() const
    {
        // ask the parent
        return _parent->contains(_index);
    }

    /**
     *  Retrieve the original value
     *  @return Value
     */
    Value value() const
    {
        return _parent->get(_index);
    }

    /**
     *  Cast to a value object
     *  @return Value
     */
    operator Value () const
    {
        return _parent->get(_index);
    }

    /**
     *  Cast to a integer
     *  @return int16_t
     */
    operator int16_t () const
    {
        return value().numericValue();
    }

    /**
     *  Cast to a integer
     *  @return int32_t
     */
    operator int32_t () const
    {
        return value().numericValue();
    }

    /**
     *  Cast to a integer
     *  @return int64_t
     */
    operator int64_t () const
    {
        return value().numericValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
        return value().boolValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return value().stringValue();
    }
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
        return value().rawValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double () const
    {
        return value().floatValue();
    }
    
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  @param  index
     *  @return HashMember
     */
    HashMember<int> operator[](int index)
    {
        return HashMember<int>(this, index);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember<std::string> operator[](const std::string &key)
    {
        return HashMember<std::string>(this, key);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember<std::string> operator[](const char *key)
    {
        return HashMember<std::string>(this, key);
    }

    /**
     *  Add a value to the object (or other arithmetric operators)
     *  @param  value
     *  @return HashMember
     */
    HashMember &operator+=(const Value &value)          { return operator=(this->value() + value); }
    HashMember &operator+=(int16_t value)               { return operator=(this->value() + value); }
    HashMember &operator+=(int32_t value)               { return operator=(this->value() + value); }
    HashMember &operator+=(int64_t value)               { return operator=(this->value() + value); }
    HashMember &operator+=(bool value)                  { return operator=(this->value() + value); }
    HashMember &operator+=(char value)                  { return operator=(this->value() + value); }
    HashMember &operator+=(const std::string &value)    { return operator=(this->value() + value); }
    HashMember &operator+=(const char *value)           { return operator=(this->value() + value); }
    HashMember &operator+=(double value)                { return operator=(this->value() + value); }

    /**
     *  Subtract a value from the object
     *  @param  value
     *  @return HashMember
     */
    HashMember &operator-=(const Value &value)          { return operator=(this->value() - value); }
    HashMember &operator-=(int16_t value)               { return operator=(this->value() - value); }
    HashMember &operator-=(int32_t value)               { return operator=(this->value() - value); }
    HashMember &operator-=(int64_t value)               { return operator=(this->value() - value); }
    HashMember &operator-=(bool value)                  { return operator=(this->value() - value); }
    HashMember &operator-=(char value)                  { return operator=(this->value() - value); }
    HashMember &operator-=(const std::string &value)    { return operator=(this->value() - value); }
    HashMember &operator-=(const char *value)           { return operator=(this->value() - value); }
    HashMember &operator-=(double value)                { return operator=(this->value() - value); }
    
    /**
     *  Multiply the object with a certain value
     *  @param  value
     *  @return HashMember
     */
    HashMember &operator*=(const Value &value)          { return operator=(this->value() * value); }
    HashMember &operator*=(int16_t value)               { return operator=(this->value() * value); }
    HashMember &operator*=(int32_t value)               { return operator=(this->value() * value); }
    HashMember &operator*=(int64_t value)               { return operator=(this->value() * value); }
    HashMember &operator*=(bool value)                  { return operator=(this->value() * value); }
    HashMember &operator*=(char value)                  { return operator=(this->value() * value); }
    HashMember &operator*=(const std::string &value)    { return operator=(this->value() * value); }
    HashMember &operator*=(const char *value)           { return operator=(this->value() * value); }
    HashMember &operator*=(double value)                { return operator=(this->value() * value); }

    /**
     *  Divide the object with a certain value
     *  @param  value
     *  @return HashMember
     */
    HashMember &operator/=(const Value &value)          { return operator=(this->value() / value); }
    HashMember &operator/=(int16_t value)               { return operator=(this->value() / value); }
    HashMember &operator/=(int32_t value)               { return operator=(this->value() / value); }
    HashMember &operator/=(int64_t value)               { return operator=(this->value() / value); }
    HashMember &operator/=(bool value)                  { return operator=(this->value() / value); }
    HashMember &operator/=(char value)                  { return operator=(this->value() / value); }
    HashMember &operator/=(const std::string &value)    { return operator=(this->value() / value); }
    HashMember &operator/=(const char *value)           { return operator=(this->value() / value); }
    HashMember &operator/=(double value)                { return operator=(this->value() / value); }

    /**
     *  Divide the object with a certain value, and get the rest
     *  @param  value
     *  @return HashMember
     */
    HashMember &operator%=(const Value &value)          { return operator=(this->value() % value); }
    HashMember &operator%=(int16_t value)               { return operator=(this->value() % value); }
    HashMember &operator%=(int32_t value)               { return operator=(this->value() % value); }
    HashMember &operator%=(int64_t value)               { return operator=(this->value() % value); }
    HashMember &operator%=(bool value)                  { return operator=(this->value() % value); }
    HashMember &operator%=(char value)                  { return operator=(this->value() % value); }
    HashMember &operator%=(const std::string &value)    { return operator=(this->value() % value); }
    HashMember &operator%=(const char *value)           { return operator=(this->value() % value); }
    HashMember &operator%=(double value)                { return operator=(this->value() % value); }
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value operator+(const Value &value)         { return this->value() + value; }
    Value operator+(int16_t value)              { return this->value() + value; }
    Value operator+(int32_t value)              { return this->value() + value; }
    Value operator+(int64_t value)              { return this->value() + value; }
    Value operator+(bool value)                 { return this->value() + value; }
    Value operator+(char value)                 { return this->value() + value; }
    Value operator+(const std::string &value)   { return this->value() + value; }
    Value operator+(const char *value)          { return this->value() + value; }
    Value operator+(double value)               { return this->value() + value; }

    /**
     *  Subtraction operator
     *  @param  value
     *  @return Value
     */
    Value operator-(const Value &value)         { return this->value() - value; }
    Value operator-(int16_t value)              { return this->value() - value; }
    Value operator-(int32_t value)              { return this->value() - value; }
    Value operator-(int64_t value)              { return this->value() - value; }
    Value operator-(bool value)                 { return this->value() - value; }
    Value operator-(char value)                 { return this->value() - value; }
    Value operator-(const std::string &value)   { return this->value() - value; }
    Value operator-(const char *value)          { return this->value() - value; }
    Value operator-(double value)               { return this->value() - value; }

    /**
     *  Multiplication operator
     *  @param  value
     *  @return Value
     */
    Value operator*(const Value &value)         { return this->value() * value; }
    Value operator*(int16_t value)              { return this->value() * value; }
    Value operator*(int32_t value)              { return this->value() * value; }
    Value operator*(int64_t value)              { return this->value() * value; }
    Value operator*(bool value)                 { return this->value() * value; }
    Value operator*(char value)                 { return this->value() * value; }
    Value operator*(const std::string &value)   { return this->value() * value; }
    Value operator*(const char *value)          { return this->value() * value; }
    Value operator*(double value)               { return this->value() * value; }

    /**
     *  Division operator
     *  @param  value
     *  @return Value
     */
    Value operator/(const Value &value)         { return this->value() / value; }
    Value operator/(int16_t value)              { return this->value() / value; }
    Value operator/(int32_t value)              { return this->value() / value; }
    Value operator/(int64_t value)              { return this->value() / value; }
    Value operator/(bool value)                 { return this->value() / value; }
    Value operator/(char value)                 { return this->value() / value; }
    Value operator/(const std::string &value)   { return this->value() / value; }
    Value operator/(const char *value)          { return this->value() / value; }
    Value operator/(double value)               { return this->value() / value; }

    /**
     *  Modulus operator
     *  @param  value
     *  @return HashMember
     */
    Value operator%(const Value &value)         { return this->value() % value; }
    Value operator%(int16_t value)              { return this->value() % value; }
    Value operator%(int32_t value)              { return this->value() % value; }
    Value operator%(int64_t value)              { return this->value() % value; }
    Value operator%(bool value)                 { return this->value() % value; }
    Value operator%(char value)                 { return this->value() % value; }
    Value operator%(const std::string &value)   { return this->value() % value; }
    Value operator%(const char *value)          { return this->value() % value; }
    Value operator%(double value)               { return this->value() % value; }
    
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
     *  Handle calls to the hash member
     *  @param  param0
     *  @param  param1
     *  @param  param2
     *  @param  param3
     *  @param  param4
     *  @param  param5
     *  @param  param6
     *  @param  param7
     *  @param  param8
     *  @param  param9
     *  @return Value
     */
    Value operator()() { return value()(); }
    Value operator()(Value param0) { return value()(param0); }
    Value operator()(Value param0, Value param1) { return value()(param0, param1); }
    Value operator()(Value param0, Value param1, Value param2) { return value()(param0, param1, param2); }
    Value operator()(Value param0, Value param1, Value param2, Value param3) { return value()(param0, param1, param2, param3); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4) { return value()(param0, param1, param2, param3, param4); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4, Value param5) { return value()(param0, param1, param2, param3, param4, param5); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4, Value param5, Value param6) { return value()(param0, param1, param2, param3, param4, param5, param6); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4, Value param5, Value param6, Value param7) { return value()(param0, param1, param2, param3, param4, param5, param6, param7); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4, Value param5, Value param6, Value param7, Value param8) { return value()(param0, param1, param2, param3, param4, param5, param6, param7, param8); }
    Value operator()(Value param0, Value param1, Value param2, Value param3, Value param4, Value param5, Value param6, Value param7, Value param8, Value param9) { return value()(param0, param1, param2, param3, param4, param5, param6, param7, param8, param9); }

    /**
     *  Check if a certain key exists in the array/object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const std::string &key) const override
    {
        // object must exist, and the value must contain the key
        return exists() && value().contains(key);
    }
    
    /**
     *  Check if a certain index exists in the array/object
     *  @param  index
     *  @return bool
     */
    virtual bool contains(int index) const override
    {
        // object must exist, and the value must contain the key
        return exists() && value().contains(index);
    }

    /**
     *  Check if a certain index exists in the array/object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const Value &key) const override
    {
        // object must exist, and the value must contain the key
        return exists() && value().contains(key);
    }
    
    /**
     *  Retrieve the value at a string index
     *  @param  key
     *  @return Value
     */
    virtual Value get(const std::string &key) const override
    {
        // return null if it does not exist
        if (!exists()) return nullptr;
        
        // ask the value
        return value().get(key);
    }
    
    /**
     *  Retrieve the value at a numeric index
     *  @param  index
     *  @return Value
     */
    virtual Value get(int index) const override
    {
        // return null if it does not exist
        if (!exists()) return nullptr;
        
        // ask the value
        return value().get(index);
    }

    /**
     *  Retrieve the value at a variant index
     *  @param  key
     *  @return Value
     */
    virtual Value get(const Value &key) const override
    {
        // return null if it does not exist
        if (!exists()) return nullptr;
        
        // ask the value
        return value().get(key);
    }
    
    /**
     *  Overwrite the value at a certain string index
     *  @param  key
     *  @param  value
     */
    virtual void set(const std::string &key, const Value &value) override
    {
        // get the current value
        Value current(this->value());
        
        // add the value
        current[key] = value;
        
        // pass this to the base
        _parent->set(_index, current);
    }
    
    /**
     *  Overwrite the value at a certain numeric index
     *  @param  index
     *  @param  value
     */
    virtual void set(int index, const Value &value) override
    {
        // get the current value
        Value current(this->value());
        
        // add the value
        current[index] = value;
        
        // pass this to the base
        _parent->set(_index, current);
    }

    /**
     *  Overwrite the value at a certain variant index
     *  @param  key
     *  @param  value
     */
    virtual void set(const Value &key, const Value &value) override
    {
        // get the current value
        Value current(this->value());
        
        // add the value
        current[key] = value;
        
        // pass this to the base
        _parent->set(_index, current);
    }

    /**
     *  Unset the member
     */
    void unset() const
    {
        _parent->unset(_index);
    }

    /**
     *  Unset a member by its index
     *  @param  index
     */
    virtual void unset(int index) override
    {
        // if the current property does not even exist, we do not have to add anything
        if (!exists()) return;
        
        // get the current value
        Value current(this->value());
        
        // skip if the property does not exist
        if (!current.contains(index)) return;
        
        // remove the index
        current.unset(index);
        
        // pass the new value to the base
        _parent->set(_index, current);
    }
    
    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const std::string &key) override
    {
        // if the current property does not even exist, we do not have to add anything
        if (!exists()) return;
        
        // get the current value
        Value current(this->value());
        
        // skip if the property does not exist
        if (!current.contains(key)) return;
        
        // remove the index
        current.unset(key);
        
        // pass the new value to the base
        _parent->set(_index, current);
    }

    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const Value &key) override
    {
        // if the current property does not even exist, we do not have to add anything
        if (!exists()) return;
        
        // get the current value
        Value current(this->value());
        
        // skip if the property does not exist
        if (!current.contains(key)) return;
        
        // remove the index
        current.unset(key);
        
        // pass the new value to the base
        _parent->set(_index, current);
    }

protected:
    /**
     *  Protected copy constructor
     *  @param  value   Other element
     */
    HashMember(const HashMember<Type> &member) : _parent(member._parent), _index(member._index) {}

    /**
     *  Move constructor
     *  @param  value   Other element
     */
//    HashMember(HashMember<Type> &&member) _NOEXCEPT : 
//        _parent(std::move(member._parent)), _index(std::move(member._index)) {}

private:
    /**
     *  Base value
     *  @var 
     */
    HashParent *_parent;
    
    /**
     *  The original index
     *  @var Type
     */
    Type _index;
    
    /**
     *  Friend classes
     */
    friend class HashMember<std::string>;
    friend class HashMember<Value>;
    friend class HashMember<int>;
    friend class Base;
    friend class Value;
};

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const HashMember<int> &value);
std::ostream &operator<<(std::ostream &stream, const HashMember<std::string> &value);

    
/**
 *  End of namespace
 */
}

