/**
 *  Value.cpp
 *
 *  Implementation for the Value class, which wraps a PHP userspace 
 *  value (a 'zval' in Zend's terminology) into a C++ object
 *
 *  Reminder for the implementer:
 * 
 *      A 'zval' is an object that represents a _value_ in the PHP user space,
 *      and thus not a variable. A 'value' or 'zval' can be used by many
 *      different variables at the same time. The 'refcount' property of the
 *      zval holds the number of variables ($a, $b, $c, et cetera) that are
 *      all linked to the same value. With this system, PHP can implement copy
 *      on write behavior.
 * 
 *      Next to the refcount, the zval also holds a is_ref property, which is
 *      set to true if all variables linked to the value are references of each
 *      other. Thus is $a, $b and $c all point to the same variable, and is_ref
 *      is set to true, changing the value means that the $a, $b and $c value
 *      are all updated. If is_res was false, a change to $a would not mean a 
 *      change to $b, and the zval should have been copied first.
 * 
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor (value = NULL)
 */
Value::Value()
{
    // create a null zval
    MAKE_STD_ZVAL(_val);
    ZVAL_NULL(_val);
}

/**
 *  Constructor for null ptr
 */
Value::Value(std::nullptr_t value)
{
    // create a null zval
    MAKE_STD_ZVAL(_val);
    ZVAL_NULL(_val);
}

/**
 *  Constructor based on integer value
 *  @param  value
 */
Value::Value(int value)
{
    // create an integer zval
    MAKE_STD_ZVAL(_val);
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on long value
 *  @param  value
 */
Value::Value(long value)
{
    // create an integer zval
    MAKE_STD_ZVAL(_val);
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on boolean value
 *  @param  value
 */
Value::Value(bool value)
{
    // create a boolean zval
    MAKE_STD_ZVAL(_val);
    ZVAL_BOOL(_val, value);
}

/**
 *  Constructor based on single character
 *  @param  value
 */
Value::Value(char value)
{
    // create a string zval
    MAKE_STD_ZVAL(_val);
    ZVAL_STRINGL(_val, &value, 1, 1);
}

/**
 *  Constructor based on string value
 *  @param  value
 */
Value::Value(const std::string &value)
{
    // create a string zval
    MAKE_STD_ZVAL(_val);
    ZVAL_STRINGL(_val, value.c_str(), value.size(), 1);
}

/**
 *  Constructor based on a byte array
 *  @param  value
 *  @param  size
 */
Value::Value(const char *value, int size)
{
    // create a string zval
    MAKE_STD_ZVAL(_val);
    ZVAL_STRINGL(_val, value, size < 0 ? strlen(value) : size, 1);
}

/**
 *  Constructor based on decimal value
 *  @param  value
 */
Value::Value(double value)
{
    // create a double zval
    MAKE_STD_ZVAL(_val);
    ZVAL_DOUBLE(_val, value);
}

/**
 *  Wrap object around zval
 *  @param  zval        Value to wrap
 *  @param  ref         Force this to be a reference
 */
Value::Value(struct _zval_struct *zval, bool ref)
{
    // just copy the zval into this object
    _val = zval;
    
    // we see ourselves as reference too
    Z_ADDREF_P(_val);
    
    // we're ready if we do not have to force it as a reference
    if (!ref || Z_ISREF_P(zval)) return;
    
    // make this a reference
    Z_SET_ISREF_P(zval);
}

/**
 *  Copy constructor
 *  @param  value
 */
Value::Value(const Value &that)
{
    // just copy the zval
    _val = that._val;

   // and we have one more reference
    Z_ADDREF_P(_val);

    // two value objects are both sharing the same zval, we turn the zval into a reference
    // @todo this is not necessarily correct
    Z_SET_ISREF_P(_val);
}

/**
 *  Move constructor
 *  @param  value
 */
Value::Value(Value &&that)
{
    // just copy the zval
    _val = that._val;

    // clear the other object
    that._val = nullptr;
}

/**
 *  Destructor
 */
Value::~Value()
{
    // ignore if moved
    if (!_val) return;
    
    // if there were two references or less, we're going to remove a reference
    // and only one reference will remain, the object will then impossible be
    // a reference
    if (Z_REFCOUNT_P(_val) <= 2) Z_UNSET_ISREF_P(_val);
    
    // destruct the zval (this function will decrement the reference counter,
    // and only destruct if there are no other references left)
    zval_ptr_dtor(&_val);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const Value &value)
{
    // skip self assignment
    if (this == &value) return *this;

	// is the object a reference?
	if (Z_ISREF_P(_val))
	{
		// the current object is a reference, this means that we should
		// keep the zval object, and copy the other value into it, get
		// the current refcount
		int refcount = Z_REFCOUNT_P(_val);
		
		// clean up the current zval (but keep the zval structure)
		zval_dtor(_val);
		
		// make the copy
		*_val = *value._val;
		zval_copy_ctor(_val);
		
		// restore refcount and reference setting
		Z_SET_ISREF_TO_P(_val, true);
		Z_SET_REFCOUNT_P(_val, refcount);
	}
	else
	{
		// destruct the zval (this function will decrement the reference counter,
		// and only destruct if there are no other references left)
		zval_ptr_dtor(&_val);

		// just copy the zval, and the refcounter
		_val = value._val;

		// and we have one more reference
		Z_ADDREF_P(_val);
	}

    // done
    return *this;
}

/**
 *  Move operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(Value &&value)
{
    // skip self assignment
    if (this == &value) return *this;

	// is the object a reference?
	if (Z_ISREF_P(_val))
	{
        // @todo difference if the other object is a reference or not?
        
        
		// the current object is a reference, this means that we should
		// keep the zval object, and copy the other value into it, get
		// the current refcount
		int refcount = Z_REFCOUNT_P(_val);
		
		// clean up the current zval (but keep the zval structure)
		zval_dtor(_val);
		
		// make the copy
		*_val = *value._val;
		
		// restore reference and refcount setting
		Z_SET_ISREF_TO_P(_val, true);
		Z_SET_REFCOUNT_P(_val, refcount);
        
        // how many references did the old variable have?
        if (Z_ISREF_P(value._val) > 1)
        {
			// the other object already had multiple references, this
			// implies that many other PHP variables are also referring 
			// to it, and we still need to store its contents, with one 
			// reference less
			Z_DELREF_P(value._val);
			
			// and we need to run the copy constructor on the current
			// value, because we're making a deep copy
			zval_copy_ctor(_val);
		}
		else
		{
			// the last and only reference to the other object was
			// removed, we no longer need it
			FREE_ZVAL(value._val);
			
			// the other object is no longer valid
			value._val = nullptr;
		}
	}
	else
	{
		// destruct the zval (this function will decrement the reference counter,
		// and only destruct if there are no other references left)
		zval_ptr_dtor(&_val);

		// just copy the zval completely
		_val = value._val;

		// the other object is no longer valid
		value._val = nullptr;
	}
	
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_LONG(_val, value);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(long value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_LONG(_val, value);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(bool value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_BOOL(_val, value);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(char value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_STRINGL(_val, &value, 1, 1);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const std::string &value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_STRINGL(_val, value.c_str(), value.size(), 1);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const char *value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);

    // set new value
    ZVAL_STRING(_val, value, 1);
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(double value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_DOUBLE(_val, value);
    
    // done
    return *this;
}

/**
 *  The type of object
 *  @return Type
 */
Type Value::type() const
{
    return (Type)Z_TYPE_P(_val);
}

/**
 *  Change the internal type
 *  @param  type
 *  @return Value
 */
Value &Value::setType(Type type)
{
    // skip if nothing changes
    if (this->type() == type) return *this;

    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);
    
    // run the conversion
    switch (type) {
    case nullType:              convert_to_null(_val); break;
    case longType:              convert_to_long(_val); break;
    case decimalType:           convert_to_double(_val); break;
    case boolType:              convert_to_boolean(_val); break;
    case arrayType:             convert_to_array(_val); break;
    case objectType:            convert_to_object(_val); break;
    case stringType:            convert_to_string(_val); break;
    }
    
    // done
    return *this;
}

/**
 *  Make a clone of the type
 *  @return Value
 */
Value Value::clone() const
{
    // the zval that will hold the copy
    zval *copy;
    
    // allocate memory
    ALLOC_ZVAL(copy);
    
    // copy the data
    INIT_PZVAL_COPY(copy, _val);
    
    // run the copy constructor to ensure that everything gets copied
    zval_copy_ctor(copy);
    
    // done
    return Value(copy);
}

/**
 *  Clone the zval to a different type
 *  @param  type
 *  @return Value
 */
Value Value::clone(Type type) const
{
    // regular clone if nothing changes
    if (this->type() == type) return clone();

    // make a clone
    return clone().setType(type);
}

/**
 *  Retrieve the value as integer
 *  @return long
 */
long Value::longValue() const
{
    // already a long?
    if (isLong()) return Z_LVAL_P(_val);
    
    // make a clone
    return clone(longType).longValue();
}

/**
 *  Retrieve the value as boolean
 *  @return bool
 */
bool Value::boolValue() const
{
    // already a bool?
    if (isBool()) return Z_BVAL_P(_val);

    // make a clone
    return clone(boolType).boolValue();
}

/**
 *  Retrieve the value as string
 *  @return string
 */
std::string Value::stringValue() const
{
    // already a string?
    if (isString()) return std::string(Z_STRVAL_P(_val), Z_STRLEN_P(_val));

    // make a clone
    return clone(stringType).stringValue();
}

/**
 *  Retrieve raw string value
 *  @return const char *
 */
const char *Value::rawValue() const
{
    // already a string?
    if (isString()) return Z_STRVAL_P(_val);
    
    // make a clone
    return clone(stringType).rawValue();
}

/**
 *  Retrieve the value as decimal
 *  @return double
 */
double Value::decimalValue() const
{
    // already a double
    if (isDecimal()) return Z_DVAL_P(_val);

    // make a clone
    return clone(decimalType).decimalValue();
}

/**
 *  The number of members in case of an array or object
 *  @return int
 */
int Value::size() const
{
    // is it an array
    if (isArray()) return zend_hash_num_elements(Z_ARRVAL_P(_val));

    // not an array, return string size if this is a string
    if (isString()) return Z_STRLEN_P(_val);
    
    // make a copy
    Value copy(*this);
    
    // convert the copy to a string
    copy.setType(decimalType);
    
    // return the string size
    return copy.size();
}

/**
 *  Does the array contain a certain index?
 *  @param  index
 *  @return bool
 */
bool Value::contains(int index) const
{
    // must be an array
    if (!isArray()) return false;
    
    // unused variable
    zval **result;
    
    // check if this index is already in the array
    return zend_hash_index_find(Z_ARRVAL_P(_val), index, (void**)&result) != FAILURE;
}

/**
 *  Does the array contain a certain key
 *  @param  key
 *  @param  size
 *  @return boolean
 */
bool Value::contains(const char *key, int size) const
{
    // must be an array
    if (!isArray()) return false;
    
    // calculate size
    if (size < 0) size = strlen(key);

    // unused variable
    zval **result;
 
    // check if index is already in the array
    return zend_hash_find(Z_ARRVAL_P(_val), key, size+1, (void **)&result) != FAILURE;
}

/**
 *  Get access to a certain array member
 *  @param  index
 *  @return Value
 */
Value Value::get(int index) const
{
    // must be an array
    if (!isArray()) return Value();
    
    // zval to retrieve
    zval **result;
 
    // check if index is in the array
    if (zend_hash_index_find(Z_ARRVAL_P(_val), index, (void **)&result) == FAILURE) return Value();
    
    // wrap the value
    return Value(*result);
}

/**
 *  Get access to a certain assoc member
 *  @param  key
 *  @param  size
 *  @return Value
 */
Value Value::get(const char *key, int size) const
{
    // must be an array
    if (!isArray()) return Value();

    // calculate size
    if (size < 0) size = strlen(key);

    // the result value
    zval **result;
    
    // check if this index is already in the array, otherwise we return NULL
    if (zend_hash_find(Z_ARRVAL_P(_val), key, size + 1, (void **)&result) == FAILURE) return Value();
    
    // wrap the value
    return Value(*result);
}

/**
 *  Set a certain property
 *  @param  index
 *  @param  value
 *  @return Value
 */
const Value &Value::set(int index, const Value &value)
{
    // the current value
    zval **current;
    
    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && zend_hash_index_find(Z_ARRVAL_P(_val), index, (void **)&current) != FAILURE)
    {
        // skip if nothing is going to change
        if (value._val == *current) return value;
    }

    // must be an array
    setType(arrayType);

    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);
    
    // add the value (this will decrement refcount on any current variable)
    add_index_zval(_val, index, value._val);

    // the variable has one more reference (the array entry)
    Z_ADDREF_P(value._val);
    
    // done
    return value;
}

/**
 *  Set a certain property
 *  @param  key
 *  @param  size
 *  @param  value
 *  @return Value
 */
const Value &Value::set(const char *key, int size, const Value &value)
{
    // the current value
    zval **current;
    
    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && zend_hash_find(Z_ARRVAL_P(_val), key, size + 1, (void **)&current) != FAILURE)
    {
        // skip if nothing is going to change
        if (value._val == *current) return value;
    }

    // must be an array
    setType(arrayType);

    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // add the value (this will reduce the refcount of the current value)
    add_assoc_zval_ex(_val, key, size+1, value._val);
    
    // the variable has one more reference (the array entry)
    Z_ADDREF_P(value._val);
    
    // done
    return value;
}

/**
 *  Array access operator
 *  This can be used for accessing arrays
 *  @param  index
 *  @return Member
 */
Member<int> Value::operator[](int index) 
{
    return Member<int>(this, index);
}

/**
 *  Array access operato
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Member
 */
Member<std::string> Value::operator[](const std::string &key) 
{
    return Member<std::string>(this, key);
}

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return Member
 */
Member<std::string> Value::operator[](const char *key) 
{
    return Member<std::string>(this, key);
}

/**
 *  Array access operator
 *  This can be used for adding a record to the array
 *  Be aware: if the 'this' object is not already an array, it will be converted into one!
 *  @param  key
 *  @return Value
 */
//Value Value::operator[]()
//{
//    
//    
//}

/**
 *  End of namespace
 */
}

