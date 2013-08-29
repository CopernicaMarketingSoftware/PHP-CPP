/**
 *  Member.h
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
namespace PhpCpp {
    
/**
 *  Forward definitions
 */
class Value;

/**
 *  Member class
 */
template <typename Type>
class Member
{
public:
    /**
     *  Destructor
     */
    virtual ~Member() {}

    /**
     *  Assign a value object to the array
     *  @param  value
     *  @return Member
     */
    Member &operator=(const Value &value)
    {
        // set property in parent array
        _base.set(_index, value);

        // if there is a parent, it should sets its value too
        if (_parent) _parent->operator=(_base);
        
        // done
        return *this;
    }

    /**
     *  Retrieve the original value
     *  @return Value
     */
    Value value() const
    {
        return _base.get(_index);
    }

    /**
     *  Cast to a value object
     *  @return Value
     */
    operator Value () const
    {
        return _base.get(_index);
    }

    /**
     *  Cast to a long
     *  @return long
     */
    operator long () const
    {
        return _base.get(_index).longValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
        return _base.get(_index).boolValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return _base.get(_index).stringValue();
    }
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
        return _base.get(_index).rawValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double () const
    {
        return _base.get(_index).decimalValue();
    }
    
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  @param  index
     *  @return Member
     */
    Member operator[](int index)
    {
        return _base.get(_index)[index].add(this);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    Member operator[](const std::string &key)
    {
        return _base.get(_index)[key].add(this);
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    Member operator[](const char *key)
    {
        return _base.get(_index)[key].add(this);
    }

private:
    /**
     *  Constructor
     *  @param  base    Base value
     *  @param  index   Index in the array
     */
    Member(const Value *base, Type index) : _base(*base), _index(index) {}
    
    /**
     *  Protected copy constructor
     *  @param  value   Other element
     */
    Member(const Member<Type> &member) : _base(member._base), _index(member._index), _parent(member._parent) {}
    
    /**
     *  Add parent
     *  @param  parent
     *  @return Member
     */
    Member &add(Member *parent)
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
    Value _base;
    
    /**
     *  Parent member (in case of nested members)
     *  @var Member
     */
    Member *_parent = nullptr;
    
    /**
     *  Only value objects may construct members
     */
    friend class Value;
    
};
    
/**
 *  End of namespace
 */
}

