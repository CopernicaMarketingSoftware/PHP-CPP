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
 *  @copyright 2013 - 2019 Copernica BV
 */
#include "includes.h"
#include "string.h"
#include "lowercase.h"

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
    ZVAL_NULL(_val);
}

/**
 *  Constructor for null ptr
*/
Value::Value(std::nullptr_t value) : Value() {}

/**
 *  Constructor based on integer value
 *  @param  value
 */
Value::Value(int16_t value)
{
    // create an integer zval
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on integer value
 *  @param  value
 */
Value::Value(int32_t value)
{
    // create an integer zval
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on int64_t value
 *  @param  value
 */
Value::Value(int64_t value)
{
    // create an integer zval
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on boolean value
 *  @param  value
 */
Value::Value(bool value)
{
    // create a boolean zval
    ZVAL_BOOL(_val, value);
}

/**
 *  Constructor based on single character
 *  @param  value
 */
Value::Value(char value)
{
    // create a string zval
    ZVAL_STRINGL(_val, &value, 1);
}

/**
 *  Constructor based on string value
 *  @param  value
 */
Value::Value(const std::string &value)
{
    // create a string zval
    ZVAL_STRINGL(_val, value.c_str(), value.size());
}

/**
 *  Constructor based on a byte array
 *  @param  value
 *  @param  size
 */
Value::Value(const char *value, int size)
{
    // is there a value?
    if (value)
    {
        // create a string zval
        ZVAL_STRINGL(_val, value, size < 0 ? ::strlen(value) : size);
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
    ZVAL_DOUBLE(_val, value);
}

/**
 *  Wrap object around zval
 *  @param  zval        Value to wrap
 *  @param  ref         Force this to be a reference
 */
Value::Value(struct _zval_struct *val, bool ref)
{
    // do we have to force a reference?
    if (!ref)
    {
        // we don't, simply duplicate the value
        ZVAL_DUP(_val, val);
    }
    else
    {
        // make sure the value is a reference
        ZVAL_MAKE_REF(val);

        // retrieve the reference
        zend_reference *ref = Z_REF_P(val);

#if PHP_VERSION_ID < 70300
        // increment refcount
        ++GC_REFCOUNT(ref);
#else
	// increment refcount
        GC_ADDREF(ref);
#endif
        // store the reference in our value
        ZVAL_REF(_val, ref);
    }
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
    if (!impl) throw Error("Assigning an unassigned object to a variable");

    // set it to an object and increase refcount
    ZVAL_OBJ(_val, impl->php());
    Z_ADDREF_P(_val);
}

/**
 *  Wrap around a php.ini value
 *  @param  value
 */
Value::Value(const IniValue &value) : Value((const char *)value) {}

/**
 *  Copy constructor
 *  @param  value
 */
Value::Value(const Value &that)
{
    // retrieve value to copy from and to
    zval* from = that._val;
    zval* to   = _val;

    // make sure it is not a reference
    ZVAL_DEREF(from);

    // copy the value
    ZVAL_COPY(to, from);
}

/**
 * Creates a reference to another Value
 *
 * Value a = b.makeReference();
 *
 * is equivalent to
 *
 * $a = &$b;
 *
 * @param to Variable to which the reference should be created
 * @return Value
 */
Value Value::makeReference()
{
    // the result value
    Value result;

    // the value we are making a reference to and the reference to create
    zval* from = _val;
    zval* to   = result._val;

    // make sure it is a reference
    ZVAL_MAKE_REF(from);

    // retriece the reference
    zend_reference *ref = Z_REF_P(from);

#if PHP_VERSION_ID < 70300
    // increment reference count
    GC_REFCOUNT(ref)++;
#else
    // increment reference count
    GC_ADDREF(ref);
#endif
    // copy the reference
    ZVAL_REF(to, ref);

    // return the result
    return result;
}

/**
 *  Move constructor
 *  @param  value
 */
Value::Value(Value &&that)  _NOEXCEPT
{
    // initialize to be undefined
    ZVAL_UNDEF(_val);

    // and then swap the values
    std::swap(_val, that._val);
}

/**
 *  Destructor
 */
Value::~Value()
{
    zval_ptr_dtor(_val);
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
 *  @param  keeprefcount
 *  @return zval
 */
Php::Zval Value::detach(bool keeprefcount)
{
    // the return value
    Php::Zval result;

    // copy the value
    ZVAL_COPY_VALUE(result, _val);

    // should we keep the reference count?
    /// FIXME: what if reference count becomes 0?
    /// Maybe we should call zval_ptr_dtor()?
    if (!keeprefcount) Z_TRY_DELREF_P(_val);

    // we no longer represent a valid value
    ZVAL_UNDEF(_val);

    // done
    return result;
}

/**
 *  Invalidate the object - so that it will not be destructed
 */
void Value::invalidate()
{
    // do nothing if object is already undefined
	if (Z_TYPE_P(_val) == IS_UNDEF) return;

    // call destructor
    zval_ptr_dtor(_val);

    // undefine it
    ZVAL_UNDEF(_val);
}

/**
 *  Retrieve the refcount
 *  @return int
 */
int Value::refcount() const
{
    // are we not a refcounted variable?
    if (!Z_REFCOUNTED_P(_val)) return 0;

    // we are, retrieve the count
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

    // if neither value is a reference we can simply swap the values
    // the other value will then destruct and reduce the refcount
    if (!Z_ISREF_P(value._val) && (!_val || !Z_ISREF_P(_val)))
    {
        // just swap the pointer
        std::swap(_val, value._val);
    }
    else if (_val)
    {
        // copy the value over to our local zval
        ZVAL_COPY_VALUE(_val, value._val);
    }
    else
    {
        // first swap the value out
        std::swap(_val, value._val);

        // and make sure it is no longer a reference
        ZVAL_UNREF(_val);
    }

    // allow chaining
    return *this;
}

/**
 *  Assign a raw zval structure
 *
 *  @param  value   The value to assign
 *  @return Value
 */
Value& Value::operator=(struct _zval_struct* value)
{
    // the value to assign to
    zval* to = _val;

    // Dereference values
    if (Z_ISREF_P(value))   value = Z_REFVAL_P(value);
    if (Z_ISREF_P(to))      to    = Z_REFVAL_P(to);

    // check if we are allowed to update this value
    if (Z_IMMUTABLE_P(to)) throw Exception("Cannot assign to an immutable variable");

    // If the destination is refcounted
    if (Z_REFCOUNTED_P(to))
    {
#if PHP_VERSION_ID < 80000
        // objects can have their own assignment handler
        if (Z_TYPE_P(to) == IS_OBJECT && Z_OBJ_HANDLER_P(to, set))
        {
            Z_OBJ_HANDLER_P(to, set)(to, value);
            return *this;
        }
#endif

        // If to and from are the same, there is nothing left to do
        if (to == value) return *this;

        // It is possible to make IS_REF point to another IS_REF, but that's a bug
        assert(Z_TYPE_P(to) != IS_REFERENCE);

        if (Z_REFCOUNT_P(to) > 1)
        {
            // If reference count is greater than 1, we need to separate zval
            // This is the optimized version of SEPARATE_ZVAL_NOREF()
            if (Z_COPYABLE_P(to))
            {
                // this will decrement the reference count and invoke GC_ZVAL_CHECK_FOR_POSSIBLE_ROOT()
                zval_ptr_dtor(to);
                zval_copy_ctor_func(to);
            }
        }
        else
        {
            // Destroy the current value of the variable and free up resources
            zval_dtor(to);
        }
    }

    // Copy the value of b to a and increment the reference count if necessary
    ZVAL_COPY(to, value);

    // allow chaining
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const Value &value)
{
    return operator=(value._val);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(std::nullptr_t value)
{
    zval z;

    ZVAL_NULL(&z);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int16_t value)
{
    zval z;

    ZVAL_LONG(&z, value);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int32_t value)
{
    zval z;

    ZVAL_LONG(&z, value);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(int64_t value)
{
    zval z;

    ZVAL_LONG(&z, value);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(bool value)
{
    zval z;

    ZVAL_BOOL(&z, value);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(char value)
{
    zval z;

    ZVAL_STRINGL(&z, &value, 1);
    operator=(&z);
    zval_dtor(&z);
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const std::string &value)
{
    zval z;

    if (value.size()) {
        ZVAL_STRINGL(&z, value.c_str(), value.size());
    }
    else {
        ZVAL_EMPTY_STRING(&z);
    }

    operator=(&z);
    zval_dtor(&z);
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const char *value)
{
    zval z;

    if (value) {
        ZVAL_STRINGL(&z, value, std::strlen(value));
    }
    else {
        ZVAL_EMPTY_STRING(&z);
    }

    operator=(&z);
    zval_dtor(&z);
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(double value)
{
    zval z;

    ZVAL_DOUBLE(&z, value);
    return operator=(&z);
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const HashMember<std::string> &value)
{
    // assign value object
    return operator=(value.value());
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Value
 */
Value &Value::operator=(const HashMember<int> &value)
{
    // assign value object
    return operator=(value.value());
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
 *  Helper function that runs the actual call
 *  @param  object      The object to call it on
 *  @param  method      The function or method to call
 *  @param  argc        Number of arguments
 *  @param  argv        The parameters
 *  @return Value
 */
static Value do_exec(const zval *object, zval *method, int argc, zval *argv)
{
    // the return zval
    zval retval;

    // remember current state of the PHP engine
    State state;
    
    // call the function
    // we're casting the const away here, object is only const so we can call this method
    // from const methods after all..
#if PHP_VERSION_ID < 80000
    if (call_user_function_ex(CG(function_table), (zval*) object, method, &retval, argc, argv, 1, nullptr) != SUCCESS)
#else
    if (call_user_function(CG(function_table), (zval*) object, method, &retval, argc, argv) != SUCCESS)
#endif
    {
        // throw an exception, the function does not exist
        throw Error("Invalid call to "+Value(method).stringValue());

        // unreachable, but let's return at least something to prevent compiler warnings
        return nullptr;
    }
    else
    {
        // the state object checks if a new exception is added to the stack, which means
        // that an exception or error occured during the call to php space
        state.rethrow();

        // leap out if nothing was returned
        if (Z_ISUNDEF(retval)) return nullptr;

        // wrap the retval in a val
        Php::Value result(&retval);

        // destruct the retval (this just decrements the refcounter, which is ok, because
        // it is already wrapped in a Php::Value so still has 1 reference)
        zval_ptr_dtor(&retval);

        // done
        return result;
    }
}

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
    return do_exec(nullptr, _val, 0, nullptr);
}

/**
 *  Is a method with the given name callable?
 *
 *  This is only applicable when the Value contains a PHP object
 *
 *  @param  name        Name of the function
 *  @return boolean
 */
bool Value::isCallable(const char *name)
{
    // this only makes sense if we are an object
    if (!isObject()) return false;

    // get the class properties
    zend_class_entry *ce = Z_OBJCE_P(_val);

    // convert the name to lowercase
    LowerCase methodname{String(name)};

    // check if the function indeed exists
    if (zend_hash_exists(&ce->function_table, methodname)) return true;

    // can we dynamically fetch the method?
    if (Z_OBJ_HT_P(_val)->get_method == nullptr) return false;

    // get the function
    union _zend_function *func = Z_OBJ_HT_P(_val)->get_method(&Z_OBJ_P(_val), methodname, nullptr);

    // if function does not exist, we do not have to check further
    if (func == nullptr) return false;

    // i dont get this code, it is copied from the method_exists() function (but the code has
    // of course been prettified because the php guys dont know how to write good looking code)
    if (!(func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) return true;

    // check the result ("Returns true to the fake Closure's __invoke")
#if PHP_VERSION_ID < 80200
    bool result = func->common.scope == zend_ce_closure && zend_string_equals_literal(methodname.value(), ZEND_INVOKE_FUNC_NAME);
#else
    bool result = func->common.scope == zend_ce_closure && zend_string_equals_cstr(methodname.value(), ZEND_INVOKE_FUNC_NAME, ::strlen(ZEND_INVOKE_FUNC_NAME));
#endif

    // free resources (still don't get this code, copied from zend_builtin_functions.c)
    zend_string_release(func->common.function_name);
    zend_free_trampoline(func);

    // done
    return result;
}

/**
 *  Call the method - if the variable holds an object with the given method
 *  @param  name        name of the method to call
 *  @return Value
 */
Value Value::call(const char *name) const
{
    // wrap the name in a Php::Value to get a zval
    Value method(name);

    // call helper function
    return do_exec(_val, method._val, 0, nullptr);
}

/**
 *  Call the method - if the variable holds an object with the given method
 *  @param  name        name of the method to call
 *  @return Value
 */
Value Value::call(const char *name)
{
    // wrap the name in a Php::Value to get a zval
    Value method(name);

    // call  helper function
    return do_exec(_val, method._val, 0, nullptr);
}

/**
 *  Call function with a number of parameters
 *  @param  argc        Number of parameters
 *  @param  argv        The parameters
 *  @return Value
 */
Value Value::exec(int argc, Value *argv) const
{
    // array of zvals to execute
    zval* params = static_cast<zval*>(alloca(argc * sizeof(zval)));

    // convert all the values
    for(int i = 0; i < argc; i++) { params[i] = *argv[i]._val; }

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
Value Value::exec(const char *name, int argc, Value *argv) const
{
    // wrap the name in a Php::Value object to get a zval
    Value method(name);

    // array of zvals to execute
    zval* params = static_cast<zval*>(alloca(argc * sizeof(zval)));

    // convert all the values
    for(int i = 0; i < argc; i++) { params[i] = *argv[i]._val; }

    // call helper function
    return do_exec(_val, method._val, argc, params);
}

/**
 *  Call method with a number of parameters
 *  @param  name        Name of method to call
 *  @param  argc        Number of parameters
 *  @param  argv        The parameters
 *  @return Value
 */
Value Value::exec(const char *name, int argc, Value *argv)
{
    // wrap the name in a Php::Value object to get a zval
    Value method(name);

    // array of zvals to execute
    zval* params = static_cast<zval*>(alloca(argc * sizeof(zval)));

    // convert all the values
    for(int i = 0; i < argc; i++) { params[i] = *argv[i]._val; }

    // call helper function
    return do_exec(_val, method._val, argc, params);
}

/**
 *  Comparison operators== for hardcoded Value
 *  @param  value
 */
bool Value::operator==(const Value &value) const
{
    // zval that will hold the result of the comparison
    zval result;

    // run the comparison
    if (SUCCESS != compare_function(&result, _val, value._val)) return false;

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
    // zval that will hold the result of the comparison
    zval result;

    // run the comparison
    if (SUCCESS != compare_function(&result, _val, value._val)) return false;

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
 *  Are we null? This will also check if we're a reference to a null value
 *  @return bool
 */
bool Value::isNull() const
{
    // if we're null we simply return true
    if (type() == Type::Null) return true;

    // derefence ourselves and check the type of that
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::Null;
}

/**
 *  Are we a number? This will also check if we're a reference to a number
 *  @return bool
 */
bool Value::isNumeric() const
{
    // if we're a number we simply return true
    if (type() == Type::Numeric) return true;

    // derefence ourselves and check the type of that
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::Numeric;
}

/**
 *  Are we a boolean? This will also check if we're a reference to a boolean
 *  @return bool
 */
bool Value::isBool() const
{
    // if we're a true or false type right away we just return true
    if (type() == Type::False || type() == Type::True) return true;

    // retrieve the type
    auto type = ((Type) Z_TYPE_P(_val.dereference()));

    // are we a true or false type?
    return type == Type::True || type == Type::False;
}

/**
 *  Are we a string? This will also check if we're a reference to a string
 *  @return bool
 */
bool Value::isString() const
{
    // if we're a string we just return true right away
    if (type() == Type::String) return true;

    // derefence ourselves and check the type of that
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::String;
}

/**
 *  Are we a float? This will also check if we're a reference to a float
 *  @return bool
 */
bool Value::isFloat() const
{
    // if we're a float ourselves we just return true right away
    if (type() == Type::Float) return true;

    // derefence ourselves and check the type of that
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::Float;
}

/**
 *  Are we a reference?
 *  @return bool
 */
bool Value::isReference() const
{
    // check our pointer if we are a reference
    return Z_ISREF_P(_val);
}

/**
 *  Are we an object? This will also check if we're a reference to an object
 *  @return bool
 */
bool Value::isObject() const
{
    // if we're an object right away we just return true
    if (type() == Type::Object) return true;

    // derefence ourselves and check the type of that
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::Object;
}

/**
 *  Are we an array? This will also check if we're a reference to an array
 *  @return bool
 */
bool Value::isArray() const
{
    // if we're directly an array we just return true
    if (type() == Type::Array) return true;

    // check the type of the dereference
    return ((Type) Z_TYPE_P(_val.dereference())) == Type::Array;
}

/**
 *  Change the internal type
 *  @param  type
 *  @return Value
 */
Value &Value::setType(Type type) &
{
    // skip if nothing changes
    if (this->type() == type) return *this;

    // if this is not a reference variable, we should detach it to implement copy on write
#if PHP_VERSION_ID >= 80100
    SEPARATE_ZVAL_NOREF(_val);
#else
    SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

    // run the conversion, when it fails we throw a fatal error that ends up in PHP space
    switch (type) {
    case Type::Undefined:       throw Error{ "Cannot make a variable undefined"                                 }; break;
    case Type::Null:            convert_to_null(_val);                                                             break;
    case Type::Numeric:         convert_to_long(_val);                                                             break;
    case Type::Float:           convert_to_double(_val);                                                           break;
    case Type::Bool:            convert_to_boolean(_val);                                                          break;
    case Type::False:           convert_to_boolean(_val); ZVAL_FALSE(_val);                                        break;
    case Type::True:            convert_to_boolean(_val); ZVAL_TRUE(_val);                                         break;
    case Type::Array:           convert_to_array(_val);                                                            break;
    case Type::Object:          convert_to_object(_val);                                                           break;
    case Type::String:          convert_to_string(_val);                                                           break;
    case Type::Resource:        throw Error{ "Resource types cannot be handled by the PHP-CPP library"          }; break;
    case Type::Constant:        throw Error{ "Constant types cannot be assigned to a PHP-CPP library variable"  }; break;
    case Type::ConstantAST:     throw Error{ "Constant types cannot be assigned to a PHP-CPP library variable"  }; break;
    case Type::Callable:        throw Error{ "Callable types cannot be assigned to a PHP-CPP library variable"  }; break;
    case Type::Reference:       throw Error{ "Reference types cannot be assigned to a PHP-CPP library variable" }; break;
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
    // we can not rely on the type, because strings can be callable as well
    return zend_is_callable(_val, 0, NULL);
}

/**
 *  Retrieve the class entry
 *  @param  allowString
 *  @return zend_class_entry
 */
zend_class_entry *Value::classEntry(bool allowString) const
{
    // is this an object
    if (isObject())
    {
        // class entry can be easily found, we try to dereference here if our
        // value is a reference to an object
        return Z_OBJCE_P(_val.dereference());
    }

    else
    {
        // the value is not an object, is this allowed?
        if (!allowString || !isString()) return nullptr;

        // find the class entry
        return zend_lookup_class(Z_STR_P(_val));
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
    // the class-entry of 'this'
    zend_class_entry *this_ce = classEntry(allowString);
    if (!this_ce) return false;

    // now we can look up the actual class
    auto *ce = zend_lookup_class_ex(String(classname, size), nullptr, 0);

    // no such class, then we are not instanceof
    if (!ce) return false;

    // check if this is a subclass
    return instanceof_function(this_ce, ce);
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
    // the class-entry of 'this'
    zend_class_entry *this_ce = classEntry(allowString);
    if (!this_ce) return false;

    // now we can look up the actual class
    auto *ce = zend_lookup_class_ex(String(classname, size), nullptr, 0);

    // unable to find the class entry?
    if (!ce) return false;

    // should not be identical, it must be a real derived object
    if (this_ce == ce) return false;

    // check if this is a subclass
    return instanceof_function(this_ce, ce);
}

/**
 *  Make a clone of the type
 *  @return Value
 */
Value Value::clone() const
{
    // value to clone to
    Value output;

    // copy the value over to the output
    ZVAL_DUP(output._val, _val);

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
    // first create the clone
    auto cloned = clone();

    // should we change the type
    if (this->type() != type) cloned.setType(type);

    // return the finished clone
    return cloned;
}

/**
 *  Retrieve the value as integer
 *  @return long
 */
int64_t Value::numericValue() const
{
    return zval_get_long(_val);
}

/**
 *  Retrieve the value as boolean
 *  @return bool
 */
bool Value::boolValue() const
{
    // what variable type do we hold?
    switch (type())
    {
        case Type::Undefined:   return false;
        case Type::Null:        return false;
        case Type::False:       return false;
        case Type::True:        return true;
        case Type::Numeric:     return numericValue();
        case Type::Float:       return floatValue();
        case Type::String:      return size();
        case Type::Array:       return size();
        default:                return clone(Type::Bool).boolValue();
    }
}

/**
 *  Retrieve the value as string
 *  @return string
 */
std::string Value::stringValue() const
{
    zend_string* s  = zval_get_string(_val);
    std::string ret(ZSTR_VAL(s), ZSTR_LEN(s));
    zend_string_release(s);
    return ret;
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
 *  Get access to the raw buffer for read operations. Note that this
 *  only works for string variables - other variables return nullptr.
 *
 *  @return const char *
 */
const char *Value::rawValue() const
{
    // must be a string
    if (isString()) return Z_STRVAL_P(_val);

    // there is no raw value
    return nullptr;
}

/**
 *  Retrieve the value as decimal
 *  @return double
 */
double Value::floatValue() const
{
    return zval_get_double(_val);
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
        zend_long result;

        // call the function
#if PHP_VERSION_ID < 80000
        return Z_OBJ_HT_P(_val)->count_elements(_val, &result) == SUCCESS ? result : 0;
#else
        zend_object *zobj = Z_OBJ_P(_val);
        return Z_OBJ_HT_P(_val)->count_elements(zobj, &result) == SUCCESS ? result : 0;
#endif
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
    if (isArray()) return ValueIterator(new HashIterator(Z_ARRVAL_P(_val.dereference()), begin, true));

    // get access to the hash table
    if (isObject())
    {
        // is a special iterator method defined in the class entry?
        auto *entry = Z_OBJCE_P(_val);

        // check if there is an iterator
        if (entry->get_iterator)
        {
            // the object implements Traversable interface, we have to use a
            // special iterator to user that interface too
            return ValueIterator(new TraverseIterator(_val, begin));
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
    // if we're an object implementing ArrayAccess it makes sense for this method to work as well, so we call offsetExists
    if (isObject() && instanceOf("ArrayAccess")) return call("offsetExists", index).boolValue();

    // must be an array
    else if (!isArray()) return false;

    // check if this index is already in the array
    return zend_hash_index_find(Z_ARRVAL_P(_val.dereference()), index) != nullptr;
}

/**
 *  Does the array contain a certain key
 *  @param  key
 *  @param  size
 *  @return
 */
bool Value::contains(const char *key, int size) const
{
    // calculate size
    if (size < 0) size = ::strlen(key);

    // deal with arrays
    if (isArray())
    {
        // check if index is already in the array
        return zend_hash_find(Z_ARRVAL_P(_val.dereference()), String(key, size)) != nullptr;
    }
    else if (isObject())
    {
#if PHP_VERSION_ID >= 70400
        // retrieve the object pointer and check whether the property we are trying to retrieve
        if (zend_check_property_access(Z_OBJ_P(_val), String(key, size), 0) == FAILURE) return false;
#else
        if (zend_check_property_access(Z_OBJ_P(_val), String(key, size)) == FAILURE) return false;
#endif
        // check if the 'has_property' method is available for this object
        auto *has_property = Z_OBJ_HT_P(_val)->has_property;

        // leap out if no 'has_property' function is not set (which should normally not occur)
        if (!has_property) return false;

        // the property must be a zval, turn the key into a value
        Value property(key, size);

        // call the has_property() method (0 means: check whether property exists and is not NULL,
        // this is not really what we want, but the closest to the possible values of that parameter)
#if PHP_VERSION_ID < 80000
        return has_property(_val, property._val, 0, nullptr);
#else
        return has_property(Z_OBJ_P(_val), zval_get_string(property._val), 0, nullptr);
#endif
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
    // if we're an actual normal array we just use the zend_hash_index_find method
    if (isArray())
    {
        // retrieve the value
        auto *result = zend_hash_index_find(Z_ARRVAL_P(_val.dereference()), index);

        // did the offset exist?
        if (!result) return Type::Undefined;

        // wrap the value
        return result;
    }
    // if we're an object implementing ArrayAccess it makes sense for this method to work as well, so we call offsetGet
    else if (isObject() && instanceOf("ArrayAccess")) return call("offsetGet", index);
    // if we're neither we return an empty value
    else return Type::Undefined;
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
        // find the result
        auto val = zend_hash_find(Z_ARRVAL_P(_val.dereference()), String(key, size));

        // wrap it in a value if it isn't null, otherwise return an empty value
        return val ? Value(val) : Value();
    }
    else
    {
        // key should not start with a null byte
        if (size > 0 && key[0] == 0) return Value();

        // temporary value for holding any error
        zval rv;

#if PHP_VERSION_ID < 70100
        zend_class_entry* scope = EG(scope);
#else
        zend_class_entry* scope = EG(fake_scope) ? EG(fake_scope) : zend_get_executed_scope();
#endif
        // read the property
#if PHP_VERSION_ID < 80000
        zval *property = zend_read_property(scope, _val, key, size, 0, &rv);
#else
        zval *property = zend_read_property(scope, Z_OBJ_P(_val), key, size, 0, &rv);
#endif

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
#if PHP_VERSION_ID >= 80100
    SEPARATE_ZVAL_NOREF(_val);
#else
    SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

    // add the value (this will decrement refcount on any current variable)
    add_index_zval(_val, index, value._val);

    // the variable has one more reference (the array entry)
    Z_TRY_ADDREF_P(value._val);
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
    zval *current;

    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && (current = zend_hash_index_find(Z_ARRVAL_P(_val.dereference()), index)))
    {
        // skip if nothing is going to change
        if (value._val == current) return;
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
#if PHP_VERSION_ID >= 80100
        SEPARATE_ZVAL_NOREF(_val);
#else
        SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

        // update the property
#if PHP_VERSION_ID < 70100
        zend_class_entry* scope = EG(scope);
#else
        zend_class_entry* scope = EG(fake_scope) ? EG(fake_scope) : zend_get_executed_scope();
#endif
#if PHP_VERSION_ID < 80000
        zend_update_property(scope, _val, key, size, value._val);
#else
        zend_update_property(scope, Z_OBJ_P(_val), key, size, value._val);
#endif
    }
    else
    {
        // if this is not a reference variable, we should detach it to implement copy on write
#if PHP_VERSION_ID >= 80100
        SEPARATE_ZVAL_NOREF(_val);
#else
        SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

        // add the value (this will reduce the refcount of the current value)
        add_assoc_zval_ex(_val, key, size, value._val);

        // the variable has one more reference (the array entry)
        Z_TRY_ADDREF_P(value._val);
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
    zval *current;

    // check if this index is already in the array, otherwise we return NULL
    if (isArray() && (current = zend_hash_find(Z_ARRVAL_P(_val.dereference()), String(key, size))))
    {
        // skip if nothing is going to change
        if (value._val == current) return;
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
#if PHP_VERSION_ID >= 80100
    SEPARATE_ZVAL_NOREF(_val);
#else
    SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

    // remove the index
    zend_hash_index_del(Z_ARRVAL_P(_val.dereference()), index);
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
#if PHP_VERSION_ID >= 80100
        SEPARATE_ZVAL_NOREF(_val);
#else
        SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

        // in the zend header files, unsetting properties is redirected to setting it to null...
        add_property_null_ex(_val, key, size);
    }
    else if (isArray())
    {
        // if this is not a reference variable, we should detach it to implement copy on write
#if PHP_VERSION_ID >= 80100
        SEPARATE_ZVAL_NOREF(_val);
#else
        SEPARATE_ZVAL_IF_NOT_REF(_val);
#endif

        // remove the index
        zend_hash_del(Z_ARRVAL_P(_val.dereference()), String(key, size));
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
    // must be an object, we try to dereference because we might be a reference
    // to an object
    if (isObject()) return ObjectImpl::find(_val.dereference())->object();

    // retrieve the mixed object that contains the base
    return nullptr;
}

/**
 * This function is used in tests to make sure that the way we assign
 * variable is consistent with that of PHP.
 *
 * @internal
 */
std::string Value::debugZval() const
{
    std::string s;
    zval* z = _val;

    s = "[type=" + std::to_string(static_cast<int>(Z_TYPE_P(z)))
      + " refcounted=" + std::to_string(Z_REFCOUNTED_P(z))
      + " isref=" + std::to_string(Z_ISREF_P(z))
      + " refcount=" + std::to_string(Z_REFCOUNTED_P(z) ? Z_REFCOUNT_P(z) : 0)
      + "] "
    ;

    zend_string* zs = zval_get_string(z);
    s += std::string(ZSTR_VAL(zs), ZSTR_LEN(zs));
    zend_string_release(zs);

    return s;
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

