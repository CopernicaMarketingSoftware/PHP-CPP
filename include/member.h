/**
 *	Member.h
 *
 *	When you're accessing members in an array or an object, you're
 *	doing so via an internal member object. This is an object that
 *	keeps track of the array to which it belonged, and that updates
 *	the class when the object is modified
 *
 *	This is an abstract class. You are not supposed to instantiate it
 *	yourself. An instance of it is created when you call Value::operator[]
 *
 *	@author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *	@copyright 2013 Copernica BV
 */

/**
 *	Set up namespace
 */
namespace PhpCpp {
	
/**
 *  Forward definitions
 */
class Value;

/**
 *  Member class
 */
template <class Type>
class Member
{
public:
	/**
	 *  Destructor
	 */
	virtual ~Member() {}

	/**
	 *  Assign a numeric value
	 *  @param	value
	 *  @return	Member
	 */
	Member &operator=(long value)
	{
		_value->set(_index, value);
		return *this;
	}

	/**
	 *  Assign a numeric value
	 *  @param	value
	 *  @return	Member
	 */
	Member &operator=(int value)
	{
		_value->set(_index, value);
		return *this;
	}
	
	/**
	 *  Assign a double value
	 *  @param	value
	 *  @return Member
	 */
	Member &operator=(double value)
	{
		_value->set(_index, value);
		return *this;
	}
	
	/**
	 *  Assign a boolean value
	 * 	@param	value
	 *  @return Member
	 */
	Member &operator=(bool value)
	{
		_value->set(_index, value);
		return *this;
	}
	
	/**
	 *  Assign a string value
	 *  @param	value
	 *  @return	Member
	 */
	Member &operator=(const std::string &value)
	{
		_value->set(_index, value);
		return *this;
	}
	
	/**
	 *  Assign a byte array value
	 *  @param	value
	 * 	@return	Member
	 */
	Member &operator=(const char *value)
	{
		_value->set(_index, value);
		return *this;
	}

	/**
	 *  Retrieve the original value
	 *  @return	Value
	 */
	Value value() const
	{
		return _value->get(_index);
	}

	/**
	 *  Cast to a value object
	 *  @return	Value
	 */
	operator Value () const
	{
		return _value->get(_index);
	}

    /**
     *  Cast to a long
     *  @return long
     */
    operator long () const
    {
		return _value->get(_index).longValue();
	}
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
		return _value->get(_index).boolValue();
	}
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
		return _value->get(_index).stringValue();
	}
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
		return _value->get(_index).rawValue();
	}
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double () const
    {
		return _value->get(_index).decimalValue();
	}
	
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  @param  index
     *  @return Member
     */
    Member operator[](int index)
    {
		return _value->get(_index)[index];
	}
    
    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Member
     */
    Member operator[](const std::string &key)
    {
		return _value->get(_index)[key];
	}

	/**
	 *  Array access operator
	 *  This can be used for accessing associative arrays
	 *  @param	key
	 *  @return	Member
	 */
	Member operator[](const char *key)
	{
		return _value->get(_index)[key];
	}

private:
	/**
	 *  Constructor
	 *	@param	value	Parent element
	 *  @param	index	Index in the array
	 */
	Member(Value *value, Type index) : _value(value), _index(index) {}
	
	/**
	 *  Private copy constructor
	 *  @param	value	Other element
	 */
	Member(const Member<Type> &member) : _value(member._value), _index(member._index) {}
	
	/**
	 *  The array of which this is a member
	 *	@var Value
	 */
	Value *_value;
	
	/**
	 *  The original index
	 *  @var Type
	 */
	Type _index;
	
	/**
	 *  Value objects may create members
	 */
	friend class Value;
};
	
/**
 *	End of namespace
 */
}

