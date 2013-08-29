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
namespace PhpCpp {

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
    // just copy the zval, and the refcounter
    _val = that._val;

    // and we have one more reference
    Z_ADDREF_P(_val);
}

/**
 *  Destructor
 */
Value::~Value()
{
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

    // destruct the zval (this function will decrement the reference counter,
    // and only destruct if there are no other references left)
    zval_ptr_dtor(&_val);

    // just copy the zval, and the refcounter
    _val = value._val;

    // and we have one more reference
    Z_ADDREF_P(_val);

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
Type Value::type()
{
    return (Type)Z_TYPE_P(_val);
}

/**
 *  Change the internal type
 *  @param  type
 */
void Value::setType(Type type)
{
    // skip if nothing changes
    if (this->type() == type) return;
    
    // run the conversion
    switch (type) {
    case nullType:              convert_to_null(_val); break;
    case intType:               convert_to_long(_val); break;
    case decimalType:           convert_to_double(_val); break;
    case boolType:              convert_to_boolean(_val); break;
    case arrayType:             convert_to_array(_val); break;
    case objectType:            convert_to_object(_val); break;
    case stringType:            convert_to_string(_val); break;
    }
}

/**
 *  Is this a NULL value?
 *  @return bool
 */
bool Value::isNull()
{
    return Z_TYPE_P(_val) == IS_NULL;
}

/**
 *  Is this an integer value?
 *  @return bool
 */
bool Value::isInt()
{
    return Z_TYPE_P(_val) == IS_LONG;
}

/**
 *  Is this a boolean value?
 *  @return bool
 */
bool Value::isBool()
{
    return Z_TYPE_P(_val) == IS_BOOL;
}

/**
 *  Is this a string value?
 *  @return bool
 */
bool Value::isString()
{
    return Z_TYPE_P(_val) == IS_STRING;
}

/**
 *  Is this a decimal value?
 *  @return bool
 */
bool Value::isDecimal()
{
    return Z_TYPE_P(_val) == IS_DOUBLE;
}

/**
 *  Is this an object value?
 *  @return bool
 */
bool Value::isObject()
{
    return Z_TYPE_P(_val) == IS_OBJECT;
}

/**
 *  Is this an array value?
 *  @return bool
 */
bool Value::isArray()
{
    return Z_TYPE_P(_val) == IS_ARRAY;
}

/**
 *  Retrieve the value as integer
 *  @return int
 */
int Value::intValue()
{
    // already an int?
    if (isInt()) return Z_LVAL_P(_val);
    
    // make a copy
    Value copy(*this);
    
    // convert the copy to an int
    copy.setType(intType);
    
    // return the int value
    return copy.intValue();
}

/**
 *  Retrieve the value as boolean
 *  @return bool
 */
bool Value::boolValue()
{
    // already a bool?
    if (isBool()) return Z_BVAL_P(_val);

    // make a copy
    Value copy(*this);
    
    // convert the copy to an int
    copy.setType(boolType);
    
    // return the int value
    return copy.intValue();
}

/**
 *  Retrieve the value as string
 *  @return string
 */
std::string Value::stringValue()
{
    // already a string?
    if (isString()) return std::string(Z_STRVAL_P(_val), Z_STRLEN_P(_val));

    // make a copy
    Value copy(*this);
    
    // convert the copy to an string
    copy.setType(stringType);
    
    // return the string value
    return copy.stringValue();
}

/**
 *  Retrieve the value as decimal
 *  @return double
 */
double Value::decimalValue()
{
    // already a double
    if (isDecimal()) return Z_DVAL_P(_val);

    // make a copy
    Value copy(*this);
    
    // convert the copy to an double
    copy.setType(decimalType);
    
    // return the decimal value
    return copy.decimalValue();
}

/**
 *  The number of members in case of an array or object
 *  @return int
 */
int Value::size()
{
    // is it an array
    if (isArray()) return zend_hash_num_elements(Z_ARRVAL_P(_val));
    
    // not an array
    return 0;
}

/**
 *  Array access operator
 *  This can be used for accessing arrays
 *  Be aware: if the 'this' object is not already an array, it will be converted into one!
 *  @param  index
 *  @return Value
 */
Value Value::operator[](int index)
{
    // must be an array
    if (!isArray()) setType(arrayType);
    
    // the result value
    zval **result;
    
    // check if this index is already in the array
    if (zend_hash_index_find(Z_ARRVAL_P(_val), index, &result) == FAILURE)
    {
        // construct a new vale
        Value val;
        
        // we want to add a new record
        add_index_zval(Z_ARRVAL_P(_val), index, val._val);

        // make the value a reference, so that changing the value will also update the array
        Z_SET_ISREF_P(val._val);
        
        // done
        return val;
    }
    else
    {
        // the index is already in the array, if multiple variables all use this
        // zval, then we want to seperate it, because the other values should
        // not be updated when the member gets updated
        SEPARATE_ZVAL_IF_NOT_REF(result);
        
        // wrap it into a value, and force this to be a reference, so that
        // changing the value will also change the array member
        return Value(*result, true);
    }
}

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  Be aware: if the 'this' object is not already an array, it will be converted into one!
 *  @param  key
 *  @return Value
 */
Value Value::operator[](const std::string &key)
{
    // must be an array
    if (!isArray()) setType(arrayType);
    
    // the result value
    zval **result;
    
    // check if this index is already in the array
    if (zend_hash_find(Z_ARRVAL_P(_val), key.c_str(), key.size() + 1, &result) == FAILURE)
    {
        // construct a new vale
        Value val;
        
        // we want to add a new record
        add_assoc_zval_ex(Z_ARRVAL_P(_val), key.c_str(), key.size()+1, val._val);

        // make the value a reference, so that changing the value will also update the array
        Z_SET_ISREF_P(val._val);
        
        // done
        return val;
    }
    else
    {
        // the index is already in the array, if multiple variables all use this
        // zval, then we want to seperate it, because the other values should
        // not be updated when the member gets updated
        SEPARATE_ZVAL_IF_NOT_REF(result);
        
        // wrap it into a value, and force this to be a reference, so that
        // changing the value will also change the array member
        return Value(*result, true);
    }
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

