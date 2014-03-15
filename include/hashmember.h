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
class HashMember
{
public:
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
        // set property in parent array
        _base->set(_index, value);

        // if there is a parent, it should sets its value too
        if (_parent) _parent->operator=(_base);
        
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
        return _base->contains(_index);
    }

    /**
     *  Retrieve the original value
     *  @return Value
     */
    Value value() const
    {
        return _base->get(_index);
    }

    /**
     *  Cast to a value object
     *  @return Value
     */
    operator Value () const
    {
        return _base->get(_index);
    }

    /**
     *  Cast to a integer
     *  @return int16_t
     */
    operator int16_t () const
    {
        return _base->get(_index).numericValue();
    }

    /**
     *  Cast to a integer
     *  @return int32_t
     */
    operator int32_t () const
    {
        return _base->get(_index).numericValue();
    }

    /**
     *  Cast to a integer
     *  @return int64_t
     */
    operator int64_t () const
    {
        return _base->get(_index).numericValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
        return _base->get(_index).boolValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return _base->get(_index).stringValue();
    }
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
        return _base->get(_index).rawValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double () const
    {
        return _base->get(_index).decimalValue();
    }
    
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  @param  index
     *  @return HashMember
     */
    HashMember operator[](int index)
    {
        return _base->get(_index)[index].add(this);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember operator[](const std::string &key)
    {
        return _base->get(_index)[key].add(this);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return HashMember
     */
    HashMember operator[](const char *key)
    {
        return _base->get(_index)[key].add(this);
    }

    /**
     *  Add a value to the object
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

private:
    /**
     *  Constructor
     *  @param  base    Base value
     *  @param  index   Index in the array
     */
    HashMember(Value *base, Type index) : _base(base), _index(index) {}
    
    // @todo add move constructor
    
    /**
     *  Protected copy constructor
     *  @param  value   Other element
     */
    HashMember(const HashMember<Type> &member) : _base(member._base), _index(member._index), _parent(member._parent) {}
    
    /**
     *  Add parent
     *  @param  parent
     *  @return HashMember
     */
    HashMember &add(HashMember *parent)
    {
        _parent = parent;
        return *this;
    }
    
    /**
     *  The original index
     *  @var Type
     */
    Type _index;
    
    /**
     *  Base value
     *  @var Value
     */
    Value *_base;
    
    /**
     *  Parent member (in case of nested members)
     *  @var HashMember
     */
    HashMember *_parent = nullptr;
    
    /**
     *  Only value objects may construct members
     */
    friend class Value;
    friend class Base;
    
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

