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
 *  @copyright 2013, 2014 Copernica BV
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
Value::Value(int16_t value)
{
    // create an integer zval
    MAKE_STD_ZVAL(_val);
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on integer value
 *  @param  value
 */
Value::Value(int32_t value)
{
    // create an integer zval
    MAKE_STD_ZVAL(_val);
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on int64_t value
 *  @param  value
 */
Value::Value(int64_t value)
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
	// allocate the zval
    MAKE_STD_ZVAL(_val);

    // is there a value?
    if (value)
    {
        // create a string zval
        ZVAL_STRINGL(_val, value, size < 0 ? ::strlen(value) : size, 1);
    }
    else
    {
        // store null
        ZVAL_NULL(_val);
    }
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
Value::Value(struct _zval_struct *val, bool ref) : _val(val)
{
    // if the variable is not already a reference, and it has more than one
    // variable pointing to it, we should seperate it so that any changes
    // we're going to make will not change the other variable
    if (ref && Z_REFCOUNT_P(_val) > 1)
    {
        // separate the zval
        SEPARATE_ZVAL_IF_NOT_REF(&_val);
    }

    // we see ourselves as reference too
    Z_ADDREF_P(_val);

    // we're ready if we do not have to force it as a reference
    if (!ref || Z_ISREF_P(_val)) return;

    // make this a reference
    Z_SET_ISREF_P(_val);
}

/**
 *  Wrap around an object
 *  @param  object
 */
Value::Value(const Base *object)
{
    // there are two options: the object was constructed from user space,
    // and is already linked to a handle, or it was constructed from C++ 
    // space, and no handle does yet exist. But if it was constructed from
    // C++ space and not yet wrapped, this Value constructor should not be 
    // called directly, but first via the derived Php::Object class. 
    auto *impl = object->implementation();
    
    // do we have a handle?
    if (!impl) throw FatalError("Assigning an unassigned object to a variable");

    // make a regular zval, and set it to an object
    MAKE_STD_ZVAL(_val);
    Z_TYPE_P(_val) = IS_OBJECT;
    Z_OBJ_HANDLE_P(_val) = impl->handle();
    
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // we have to lookup the object in the object-table
    zend_object_store_bucket *obj_bucket = &EG(objects_store).object_buckets[impl->handle()];
    
    // there is one more reference to the object
    obj_bucket->bucket.obj.refcount += 1;
    
    // this is copy-pasted from zend_objects.c - and it is necessary too!
    if (!obj_bucket->bucket.obj.handlers) obj_bucket->bucket.obj.handlers = &std_object_handlers;
    
    // store the handlers in the zval too (cast is necessary for php 5.3)
    Z_OBJ_HT_P(_val) = (zend_object_handlers*)obj_bucket->bucket.obj.handlers;
}

/**
 *  Wrap around a php.ini value
 *  @param  value
 */
Value::Value(const IniValue &value) : Value((const char *)value)
{
}

/**
 *  Copy constructor
 *  @param  value
 */
Value::Value(const Value &that)
{
    // is the other variable a reference?
    if (Z_ISREF_P(that._val))
    {
        // because this is supposed to be a COPY, we can not add ourselves
        // to the variable but have to allocate a new variable
        ALLOC_ZVAL(_val);
        INIT_PZVAL_COPY(_val, that._val);
        
        // we have to call the copy constructor to copy the entire other zval
        zval_copy_ctor(_val);
    }
    else
    {
        // simply use the same zval
        _val = that._val;
    }
    
    // that zval has one more reference
    Z_ADDREF_P(_val);


//  Below the old implementation - I thought really hard about it and I though
//  it was a correct and very smart implementation. However, it does not work
//  when you swap two variables. I changed it to the implementation above, but
//  maybe that implementation introduces other bugs??? Let's keep the old
//  implementation for a while in this file, but commented out
//
//    // how many references does the other object have?
//    if (Z_REFCOUNT_P(that._val) > 1 && !Z_ISREF_P(that._val))
//    {
//        // there are already multiple variables linked to this value, and it
//        // is not a reference. this implies that we can not turn this variable
//        // into a reference, otherwise strange things could happen, we're going
//        // to create a new zval
//        ALLOC_ZVAL(_val);
//        INIT_PZVAL_COPY(_val, that._val);
//        zval_copy_ctor(_val);
//    }
//    else
//    {
//        // simply use the same zval
//        _val = that._val;
//    }
//        
//    // the other object only has one variable using it, or it is already
//    // a variable by reference, we can safely add one more reference to it
//    // and make it a variable by reference if it was not already a ref
//    Z_ADDREF_P(_val);
//
//    // make reference
//    Z_SET_ISREF_P(_val);
}

/**
 *  Move constructor
 *  @param  value
 */
Value::Value(Value &&that)  _NOEXCEPT: _val(that._val)
{
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
zval *Value::detach()
{
    // leap out if already detached
    if (!_val) return nullptr;
    
    // copy return value
    zval *result = _val;
    
    // decrement reference counter
    Z_DELREF_P(_val);
    
    // reset internal object
    _val = nullptr;
    
    // done
    return result;
}

/**
 *  Attach a different zval
 * 
 *  This will first detach the current zval, and link the Value object to 
 *  a different zval.
 * 
 *  @param  val
 */
void Value::attach(struct _zval_struct *val)
{
    // detach first
    if (_val) detach();
    
    // store the zval
    _val = val;
    
    // add one more reference
    Z_ADDREF_P(_val);
}

/**
 *  Attach a different zval
 * 
 *  This will first detach the current zval, and link the Value object to 
 *  a new zval
 * 
 *  @param  hashtable
 */
void Value::attach(struct _hashtable *hashtable)
{
    // detach first
    if (_val) detach();

    // construct a new zval
    MAKE_STD_ZVAL(_val);
    
    // store pointer to the hashtable, and mark the zval as an array
    Z_ARRVAL_P(_val) = hashtable;
    Z_TYPE_P(_val) = IS_ARRAY;

    // add a reference
    Z_ADDREF_P(_val);
}

/**
 *  Retrieve the refcount
 *  @return int
 */
int Value::refcount() const
{
    return Z_REFCOUNT_P(_val);
}

/**
 *  Move operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(Value &&value) _NOEXCEPT
{
    // skip self assignment
    if (this == &value) return *this;

    // is the object a reference?
    if (_val && Z_ISREF_P(_val))
    {
        // @todo difference if the other object is a reference or not?
        
        // the current object is a reference, this means that we should
        // keep the zval object, and copy the other value into it, get
        // the current refcount
        int refcount = Z_REFCOUNT_P(_val);
        
        // make the copy
        *_val = *value._val;
        
        // restore reference and refcount setting
        Z_SET_ISREF_TO_P(_val, true);
        Z_SET_REFCOUNT_P(_val, refcount);
        
        // how many references did the old variable have?
        if (Z_REFCOUNT_P(value._val) > 1)
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
            // we need the tsrm_ls variable
            TSRMLS_FETCH();
            
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
        if (_val) zval_ptr_dtor(&_val);

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
    
    // update the object
    return *this;
}


/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(std::nullptr_t value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);

    // change to null value
    ZVAL_NULL(_val);

    // update the object
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int16_t value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_LONG(_val, value);

    // update the object
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int32_t value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_LONG(_val, value);

    // update the object
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int64_t value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);

    // deallocate current zval (without cleaning the zval structure)
    zval_dtor(_val);
    
    // set new value
    ZVAL_LONG(_val, value);

    // update the object
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

    // update the object
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
    
    // update the object
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
    
    // update the object
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
    
    // update the object
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
    
    // update the object
    return *this;
}

/**
 *  Add a value to the object
 *  @param  value
 *  @return Value
 */
Value &Value::operator+=(const Value &value)        { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(int16_t value)             { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(int32_t value)             { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(int64_t value)             { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(bool value)                { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(char value)                { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(const std::string &value)  { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(const char *value)         { return Arithmetic<std::plus>(this).assign(value); }
Value &Value::operator+=(double value)              { return Arithmetic<std::plus>(this).assign(value); }

/**
 *  Subtract a value from the object
 *  @param  value
 *  @return Value
 */
Value &Value::operator-=(const Value &value)        { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(int16_t value)             { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(int32_t value)             { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(int64_t value)             { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(bool value)                { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(char value)                { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(const std::string &value)  { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(const char *value)         { return Arithmetic<std::minus>(this).assign(value); }
Value &Value::operator-=(double value)              { return Arithmetic<std::minus>(this).assign(value); }

/**
 *  Multiply the object with a certain value
 *  @param  value
 *  @return Value
 */
Value &Value::operator*=(const Value &value)        { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(int16_t value)             { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(int32_t value)             { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(int64_t value)             { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(bool value)                { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(char value)                { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(const std::string &value)  { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(const char *value)         { return Arithmetic<std::multiplies>(this).assign(value); }
Value &Value::operator*=(double value)              { return Arithmetic<std::multiplies>(this).assign(value); }

/**
 *  Divide the object with a certain value
 *  @param  value
 *  @return Value
 */
Value &Value::operator/=(const Value &value)        { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(int16_t value)             { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(int32_t value)             { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(int64_t value)             { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(bool value)                { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(char value)                { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(const std::string &value)  { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(const char *value)         { return Arithmetic<std::divides>(this).assign(value); }
Value &Value::operator/=(double value)              { return Arithmetic<std::divides>(this).assign(value); }

/**
 *  Divide the object with a certain value and get the rest
 *  Note that this does not use the Arithmetic object, because no conversion between floats is necessary
 *  @param  value
 *  @return Value
 */
Value &Value::operator%=(const Value &value)        { return operator=(numericValue() % value.numericValue()); }
Value &Value::operator%=(int16_t value)             { return operator=(numericValue() % value); }
Value &Value::operator%=(int32_t value)             { return operator=(numericValue() % value); }
Value &Value::operator%=(int64_t value)             { return operator=(numericValue() % value); }
Value &Value::operator%=(bool value)                { return operator=(numericValue() % value); }
Value &Value::operator%=(char value)                { return operator=(numericValue() % value); }
Value &Value::operator%=(const std::string &value)  { return operator=(numericValue() % atoi(value.c_str())); }
Value &Value::operator%=(const char *value)         { return operator=(numericValue() % atoi(value)); }
Value &Value::operator%=(double value)              { return operator=(numericValue() % (int)value); }

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value Value::operator+(const Value &value)          { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(int16_t value)               { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(int32_t value)               { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(int64_t value)               { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(bool value)                  { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(char value)                  { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(const std::string &value)    { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(const char *value)           { return Arithmetic<std::plus>(this).apply(value); }
Value Value::operator+(double value)                { return Arithmetic<std::plus>(this).apply(value); }

/**
 *  Subtraction operator
 *  @param  value
 *  @return Value
 */
Value Value::operator-(const Value &value)          { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(int16_t value)               { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(int32_t value)               { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(int64_t value)               { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(bool value)                  { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(char value)                  { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(const std::string &value)    { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(const char *value)           { return Arithmetic<std::minus>(this).apply(value); }
Value Value::operator-(double value)                { return Arithmetic<std::minus>(this).apply(value); }

/**
 *  Multiplication operator
 *  @param  value
 *  @return Value
 */
Value Value::operator*(const Value &value)          { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(int16_t value)               { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(int32_t value)               { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(int64_t value)               { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(bool value)                  { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(char value)                  { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(const std::string &value)    { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(const char *value)           { return Arithmetic<std::multiplies>(this).apply(value); }
Value Value::operator*(double value)                { return Arithmetic<std::multiplies>(this).apply(value); }

/**
 *  Division operator
 *  @param  value
 *  @return Value
 */
Value Value::operator/(const Value &value)          { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(int16_t value)               { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(int32_t value)               { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(int64_t value)               { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(bool value)                  { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(char value)                  { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(const std::string &value)    { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(const char *value)           { return Arithmetic<std::divides>(this).apply(value); }
Value Value::operator/(double value)                { return Arithmetic<std::divides>(this).apply(value); }

/**
 *  Modulus operator
 *  @param  value
 *  @return Value
 */
Value Value::operator%(const Value &value)          { return Value(numericValue() % value.numericValue()); }
Value Value::operator%(int16_t value)               { return Value(numericValue() % value); }
Value Value::operator%(int32_t value)               { return Value(numericValue() % value); }
Value Value::operator%(int64_t value)               { return Value(numericValue() % value); }
Value Value::operator%(bool value)                  { return Value(numericValue() % value); }
Value Value::operator%(char value)                  { return Value(numericValue() % value); }
Value Value::operator%(const std::string &value)    { return Value(numericValue() % atoi(value.c_str())); }
Value Value::operator%(const char *value)           { return Value(numericValue() % atoi(value)); }
Value Value::operator%(double value)                { return Value(numericValue() % (int)value); }

/**
 *  Call the function in PHP
 *  We have ten variants of this function, depending on the number of parameters
 *  This call operator is only useful when the variable represents a callable
 *  @param  p0-p10          Parameters of the function to be called.
 *  @return Value
 */
Value Value::operator()() const
{
    // call with zero parameters
    return exec(0, NULL);
}

/**
 *  Call the method - if the variable holds an object with the given method
 *  @param  name        name of the method to call
 *  @return Value
 */
Value Value::call(const char *name)
{
    // call with zero parameters
    return exec(name, 0, NULL);
}

/**
 *  Helper function that runs the actual call
 *  @param  object      The object to call it on
 *  @param  method      The function or method to call
 *  @param  args        Number of arguments
 *  @param  params      The parameters
 *  @return Value
 */
static Value do_exec(zval **object, zval *method, int argc, zval ***params)
{
    // the return zval
    zval *retval = nullptr;

    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the current exception
    zval *oldException = EG(exception);
    
    // call the function
    if (call_user_function_ex(CG(function_table), object, method, &retval, argc, params, 1, NULL TSRMLS_CC) != SUCCESS)
    {
        // throw an exception, the function does not exist
        throw Exception("Invalid call to "+Value(method).stringValue());
        
        // unreachable, but let's return at least something to prevent compiler warnings
        return nullptr;
    }
    else
    {
        // was an exception thrown inside the function? In that case we throw a C++ new exception 
        // to give the C++ code the chance to catch it
        if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

        // no (additional) exception was thrown
        return retval ? Value(retval) : nullptr;
    }
}

/**
 *  Call function with a number of parameters
 *  @param  argc        Number of parameters
 *  @param  argv        The parameters
 *  @return Value
 */
Value Value::exec(int argc, zval ***params) const
{
    // call helper function
    return do_exec(nullptr, _val, argc, params);
}

/**
 *  Call method with a number of parameters
 *  @param  name        Name of method to call
 *  @param  argc        Number of parameters
 *  @param  argv        The parameters
 *  @return Value
 */
Value Value::exec(const char *name, int argc, struct _zval_struct ***params)
{
    // wrap the name in a Php::Value object to get a zval
    Value method(name);
    
    // call helper function
    return do_exec(&_val, method._val, argc, params);
}

/**
 *  Comparison operators== for hardcoded Value
 *  @param  value
 */
bool Value::operator==(const Value &value) const
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // zval that will hold the result of the comparison
    zval result;
    
    // run the comparison
    if (SUCCESS != compare_function(&result, _val, value._val TSRMLS_CC)) return false;
    
    // convert to boolean
    return result.value.lval == 0;
}

/**
 *  Comparison operators< for hardcoded Value
 *  @param  value
 *  @return bool
 */
bool Value::operator<(const Value &value) const
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // zval that will hold the result of the comparison
    zval result;
    
    // run the comparison
    if (SUCCESS != compare_function(&result, _val, value._val TSRMLS_CC)) return false;
    
    // convert to boolean
    return result.value.lval < 0;
}

/**
 *  The type of object
 *  @return Type
 */
Type Value::type() const
{
    // return regular type
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
    
    // run the conversion, when it fails we throw a fatal error which will
    // in the end result in a zend_error() call. This FatalError class is necessary
    // because a direct call to zend_error() will do a longjmp() which may not
    // clean up the C++ objects created by the extension
    switch (type) {
    case Type::Null:            convert_to_null(_val); break;
    case Type::Numeric:         convert_to_long(_val); break;
    case Type::Float:           convert_to_double(_val); break;
    case Type::Bool:            convert_to_boolean(_val); break;
    case Type::Array:           convert_to_array(_val); break;
    case Type::Object:          convert_to_object(_val); break;
    case Type::String:          convert_to_string(_val); break;
    case Type::Resource:        throw FatalError("Resource types can not be handled by the PHP-CPP library"); break;
    case Type::Constant:        throw FatalError("Constant types can not be assigned to a PHP-CPP library variable"); break;
    case Type::ConstantArray:   throw FatalError("Constant types can not be assigned to a PHP-CPP library variable"); break;
    case Type::Callable:        throw FatalError("Callable types can not be assigned to a PHP-CPP library variable"); break;
    }
    
    // done
    return *this;
}

/**
 *  Check if the variable holds something that is callable
 *  @return bool
 */ 
bool Value::isCallable() const
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // we can not rely on the type, because strings can be callable as well
    return zend_is_callable(_val, 0, NULL TSRMLS_CC);
}

/**
 *  Retrieve the class entry
 *  @param  allowString
 *  @return zend_class_entry
 */
zend_class_entry *Value::classEntry(bool allowString) const
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // is this an object
    if (isObject())
    {
        // should have a class entry
        if (!HAS_CLASS_ENTRY(*_val)) return nullptr;
        
        // class entry can be easily found
        return Z_OBJCE_P(_val);
    }
    else
    {
        // the value is not an object, is this allowed?
        if (!allowString || !isString()) return nullptr;
        
        // temporary variable
        zend_class_entry **ce;
        
        // find the class entry
        if (zend_lookup_class(Z_STRVAL_P(_val), Z_STRLEN_P(_val), &ce TSRMLS_CC) == FAILURE) return nullptr;
    
        // found the entry
        return *ce;
    }
}

/**
 *  Check whether this object is an instance of a certain class
 *
 *  If you set the parameter 'allowString' to true, and the Value object
 *  holds a string, the string will be treated as class name.
 *
 *  @param  classname   The class of which this should be an instance
 *  @param  size        Length of the classname string
 *  @param  allowString Is it allowed for 'this' to be a string
 *  @return bool
 */
bool Value::instanceOf(const char *classname, size_t size, bool allowString) const 
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the class-entry of 'this'
    zend_class_entry *this_ce = classEntry(allowString);
    if (!this_ce) return false;

    // class entry of the parameter
    zend_class_entry **ce;

    // now we can look up the actual class
    // the signature of zend_lookup_class_ex is slightly different since 5.4
    // TODO The signature of this changed once again as of 5.6!
#if PHP_VERSION_ID >= 50400
    if (zend_lookup_class_ex(classname, size, NULL, 0, &ce TSRMLS_CC) == FAILURE) return false;
#else
    if (zend_lookup_class_ex(classname, size, 0, &ce TSRMLS_CC) == FAILURE) return false;
#endif

    // check if this is a subclass
    return instanceof_function(this_ce, *ce TSRMLS_CC);
}

/**
 *  Check whether this object is derived from a certain class
 *
 *  If you set the parameter 'allowString' to true, and the Value object
 *  holds a string, the string will be treated as class name.
 *
 *  @param  classname   The class of which this should be an instance
 *  @param  size        Length of the classname string
 *  @param  allowString Is it allowed for 'this' to be a string
 *  @return bool
 */
bool Value::derivedFrom(const char *classname, size_t size, bool allowString) const 
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the class-entry of 'this'
    zend_class_entry *this_ce = classEntry(allowString);
    if (!this_ce) return false;

    // class entry of the parameter
    zend_class_entry **ce;

    // now we can look up the actual class
    // the signature of zend_lookup_class_ex is slightly different since 5.4
    // TODO The signature of this changed once again as of 5.6!
#if PHP_VERSION_ID >= 50400
    if (zend_lookup_class_ex(classname, size, NULL, 0, &ce TSRMLS_CC) == FAILURE) return false;
#else
    if (zend_lookup_class_ex(classname, size, 0, &ce TSRMLS_CC) == FAILURE) return false;
#endif

    // should not be identical, it must be a real derived object
    if (this_ce == *ce) return false;

    // check if this is a subclass
    return instanceof_function(this_ce, *ce TSRMLS_CC);
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

    // wrap it using the Value(zval*) constructor, this will +1 the refcount!!!!
    Value output(copy);

    // -1 the refcount to avoid future leaks
    Z_DELREF_P(copy);

    // done
    return output;
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
int64_t Value::numericValue() const
{
    // already a long?
    if (isNumeric()) return Z_LVAL_P(_val);
    
    // make a clone
    return clone(Type::Numeric).numericValue();
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
    return clone(Type::Bool).boolValue();
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
    return clone(Type::String).stringValue();
}

/**
 *  Access to the raw buffer
 *  @return char *
 */
char *Value::buffer() const
{
    // must be a string
    if (!isString()) return nullptr;
    
    // already a string?
    return Z_STRVAL_P(_val);
}

/**
 *  Reserve enough space
 *  @param  size
 *  @return char*
 */
char *Value::reserve(size_t size)
{
    // must be a string
    setType(Type::String);
 
    // is the current buffer too small?
    if (Z_STRLEN_P(_val) < (int)size)
    {
        // is there already a buffer?
        if (!Z_STRVAL_P(_val)) Z_STRVAL_P(_val) = (char *)emalloc(size+1);
        
        // reallocate an existing buffer
        else Z_STRVAL_P(_val) = (char *)erealloc(Z_STRVAL_P(_val), size+1);

        // last byte should be zero
        Z_STRVAL_P(_val)[size] = 0;
    }
    
    // store size
    Z_STRLEN_P(_val) = size;
    
    // done
    return Z_STRVAL_P(_val);
}

/**
 *  Access to the raw buffer
 *  @return const char *
 */
const char *Value::rawValue() const
{
    // must be a string
    if (isString()) return Z_STRVAL_P(_val);
    
    // make a clone and return that buffer
    return clone(Type::String).rawValue();
}

/**
 *  Retrieve the value as decimal
 *  @return double
 */
double Value::floatValue() const
{
    // already a double
    if (isFloat()) return Z_DVAL_P(_val);

    // make a clone
    return clone(Type::Float).floatValue();
}

/**
 *  The number of members in case of an array or object
 *  @return int
 */
int Value::size() const
{
    // is it an array?
    if (isArray()) 
    {
        // get the number of elements
        return zend_hash_num_elements(Z_ARRVAL_P(_val));
    }

    // or an object?
    else if (isObject())
    {
        // the count_elements member function should be defined
        if (!Z_OBJ_HT_P(_val)->count_elements) return 0;
        
        // create a variable to hold the result
        long result;
        
        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        
        // call the function
        return Z_OBJ_HT_P(_val)->count_elements(_val, &result TSRMLS_CC) == SUCCESS ? result : 0;
    }

    // not an array, return string size if this is a string
    else if (isString()) 
    {
        // get string size
        return Z_STRLEN_P(_val);
    }
    
    // in all other situations, we convert the variable to a string
    else
    {
        // make a copy
        Value copy(*this);
        
        // convert the copy to a string
        copy.setType(Type::String);
        
        // return the string size
        return copy.size();
    }
}

/**
 *  Convert the object to a map with string index and Php::Value value
 *  @return std::map
 */
std::map<std::string,Php::Value> Value::mapValue() const
{
    // result variable
    std::map<std::string,Php::Value> result;

    // iterate over the object
    for (auto &iter : *this) result[iter.first.stringValue()] = iter.second;

    // done
    return result;
}

/**
 *  Internal helper method to retrieve an iterator
 *  @param  begin       Should the iterator start at the begin
 *  @return iterator
 */
ValueIterator Value::createIterator(bool begin) const
{
    // check type
    if (isArray()) return ValueIterator(new HashIterator(Z_ARRVAL_P(_val), begin, true));
    
    // get access to the hash table
    if (isObject()) 
    {
        // we need the TSRMLS_CC variable
        TSRMLS_FETCH();
        
        // is a special iterator method defined in the class entry?
        auto *entry = zend_get_class_entry(_val TSRMLS_CC);
        
        // check if there is an iterator
        if (entry->get_iterator)
        {
            // the object implements Traversable interface, we have to use a
            // special iterator to user that interface too
            return ValueIterator(new TraverseIterator(_val, begin TSRMLS_CC));
        }
        else
        {
            // construct a regular iterator
            return ValueIterator(new HashIterator(Z_OBJPROP_P(_val), begin));
        }
    }
    
    // invalid
    return ValueIterator(new InvalidIterator());
}

/**
 *  Return an iterator for iterating over the values
 *  This is only meaningful for Value objects that hold an array or an object
 *  @return iterator
 */
ValueIterator Value::begin() const
{
    return createIterator(true);
}

/**
 *  Return an iterator for iterating over the values
 *  This is only meaningful for Value objects that hold an array or an object
 *  @return iterator
 */
ValueIterator Value::end() const
{
    return createIterator(false);
}

/**
 *  Iterate over key value pairs
 *  @param  callback
 */
void Value::iterate(const std::function<void(const Php::Value &,const Php::Value &)> &callback) const
{
    // iterate over the object
    for (const auto &iter : *this)
    {
        // call the callback
        callback(iter.first, iter.second);
    }
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
    // calculate size
    if (size < 0) size = ::strlen(key);

    // deal with arrays
    if (isArray())
    {
        // unused variable
        zval **result;
     
        // check if index is already in the array
        return zend_hash_find(Z_ARRVAL_P(_val), key, size+1, (void **)&result) != FAILURE;
    }
    else if (isObject())
    {
        // we need the tsrmls_cc variable
        TSRMLS_FETCH();
        
        // retrieve the class entry
        auto *entry = zend_get_class_entry(_val TSRMLS_CC);
        
        // read the property (cast necessary for php 5.3)
        zval *property = zend_read_property(entry, _val, (char *)key, size, 0 TSRMLS_CC);
        
        // check if valid
        return property != nullptr;
    }
    else
    {
        // scalar variable
        return false;
    }
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
    // must be an array or object
    if (!isArray() && !isObject()) return Value();

    // calculate size
    if (size < 0) size = ::strlen(key);
    
    // are we in an object or an array?
    if (isArray())
    {
        // the result value
        zval **result;
        
        // check if this index is already in the array, otherwise we return NULL
        if (zend_hash_find(Z_ARRVAL_P(_val), key, size + 1, (void **)&result) == FAILURE) return Value();
        
        // wrap the value
        return Value(*result);
    }
    else
    {
        // key should not start with a null byte
        if (size > 0 && key[0] == 0) return Value();
        
        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        
        // retrieve the class entry
        auto *entry = zend_get_class_entry(_val TSRMLS_CC);
        
        // read the property (case necessary for php 5.3)
        zval *property = zend_read_property(entry, _val, (char *)key, size, 1 TSRMLS_CC);
        
        // wrap in value
        return Value(property);
    }
}

/**
 *  Set a certain property without performing any checks
 *  This method can be used when it is already known that the object is an array
 *  @param  index
 *  @param  value
 *  @return Value
 */
void Value::setRaw(int index, const Value &value)
{
    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);
    
    // add the value (this will decrement refcount on any current variable)
    add_index_zval(_val, index, value._val);

    // the variable has one more reference (the array entry)
    Z_ADDREF_P(value._val);
}

/**
 *  Set a certain property
 *  @param  index
 *  @param  value
 *  @return Value
 */
void Value::set(int index, const Value &value)
{
    // the current value
    zval **current;
    
    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && zend_hash_index_find(Z_ARRVAL_P(_val), index, (void **)&current) != FAILURE)
    {
        // skip if nothing is going to change
        if (value._val == *current) return;
    }

    // must be an array
    setType(Type::Array);

    // set property
    setRaw(index, value);
}

/**
 *  Set a certain property without running any checks
 *  @param  key
 *  @param  size
 *  @param  value
 */
void Value::setRaw(const char *key, int size, const Value &value)
{
    // does not work for empty keys
    if (!key || (size > 0 && key[0] == 0)) return;
    
    // is this an object?
    if (isObject())
    {
        // if this is not a reference variable, we should detach it to implement copy on write
        SEPARATE_ZVAL_IF_NOT_REF(&_val);

        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        // retrieve the class entry
        auto *entry = zend_get_class_entry(_val TSRMLS_CC);
        
        // update the property (cast necessary for php 5.3)
        zend_update_property(entry, _val, (char *)key, size, value._val TSRMLS_CC);
    }
    else
    {
        // if this is not a reference variable, we should detach it to implement copy on write
        SEPARATE_ZVAL_IF_NOT_REF(&_val);

        // add the value (this will reduce the refcount of the current value)
        add_assoc_zval_ex(_val, key, size+1, value._val);

        // the variable has one more reference (the array entry)
        Z_ADDREF_P(value._val);
    }
}

/**
 *  Set a certain property
 *  @param  key
 *  @param  size
 *  @param  value
 *  @return Value
 */
void Value::set(const char *key, int size, const Value &value)
{
    // the current value
    zval **current;
    
    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && zend_hash_find(Z_ARRVAL_P(_val), key, size + 1, (void **)&current) != FAILURE)
    {
        // skip if nothing is going to change
        if (value._val == *current) return;
    }
    
    // this should be an object or an array
    if (!isObject()) setType(Type::Array);
    
    // done
    setRaw(key, size, value);
}

/**
 *  Unset a member by its index
 *  @param  index
 */
void Value::unset(int index)
{
    // only necessary for arrays
    if (!isArray()) return;

    // if this is not a reference variable, we should detach it to implement copy on write
    SEPARATE_ZVAL_IF_NOT_REF(&_val);
    
    // remove the index
    zend_hash_index_del(Z_ARRVAL_P(_val), index);
}

/**
 *  Unset by key name and length of the key
 *  @param  key
 *  @param  size
 */
void Value::unset(const char *key, int size)
{
    // is this an object?
    if (isObject())
    {
        // if this is not a reference variable, we should detach it to implement copy on write
        SEPARATE_ZVAL_IF_NOT_REF(&_val);

        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        // in the zend header files, unsetting properties is redirected to setting it to null...
        add_property_null_ex(_val, key, size TSRMLS_CC);
    }
    else if (isArray())
    {
        // if this is not a reference variable, we should detach it to implement copy on write
        SEPARATE_ZVAL_IF_NOT_REF(&_val);

        // remove the index
        zend_hash_del(Z_ARRVAL_P(_val), key, size + 1);
    }
}

/**
 *  Array access operator
 *  This can be used for accessing arrays
 *  @param  index
 *  @return HashMember
 */
HashMember<int> Value::operator[](int index) 
{
    return HashMember<int>(this, index);
}

/**
 *  Index by other value object
 *  @param  key
 *  @return HashMember<std::string>
 */
HashMember<Value> Value::operator[](const Value &key)
{
    return HashMember<Value>(this, key);
}

/**
 *  Array access operato
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return HashMember
 */
HashMember<std::string> Value::operator[](const std::string &key) 
{
    return HashMember<std::string>(this, key);
}

/**
 *  Array access operator
 *  This can be used for accessing associative arrays
 *  @param  key
 *  @return HashMember
 */
HashMember<std::string> Value::operator[](const char *key) 
{
    return HashMember<std::string>(this, key);
}

/**
 *  Retrieve the original implementation
 * 
 *  This only works for classes that were implemented using PHP-CPP,
 *  it returns nullptr for all other classes
 * 
 *  @return Base*
 */
Base *Value::implementation() const
{
    // must be an object
    if (!isObject()) return nullptr;
    
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // retrieve the mixed object that contains the base
    return ObjectImpl::find(_val TSRMLS_CC)->object();
}

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const Value &value)
{
    return stream << value.stringValue();
}

/**
 *  End of namespace
 */
}

