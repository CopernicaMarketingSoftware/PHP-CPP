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
    
    // should this be a forced reference
    if (ref) Z_SET_ISREF_P(zval);
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
 *  End of namespace
 */
}

