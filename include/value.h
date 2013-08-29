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
namespace PhpCpp {

/**
 *  Class definition
 */
class Value
{
public:
    /**
     *  Empty constructor (value = NULL)
     */
    Value();
    
    /**
     *  Constructor based on integer value
     *  @param  value
     */
    Value(int value);
    
    /**
     *  Constructor based on boolean value
     *  @param  value
     */
    Value(bool value);
    
    /**
     *  Constructor based on string value
     *  @param  value
     */
    Value(const std::string &value);
    
    /**
     *  Constructor based on decimal value
     *  @param  value
     */
    Value(double value);
    
    /**
     *  Wrap object around zval
     *  @param  zval        Zval to wrap
     *  @param  ref         Force this to be a reference
     */
    Value(struct _zval_struct *zval, bool ref = false);
    
    /**
     *  Copy constructor
     *  @param  value
     */
    Value(const Value &that);
    
    /**
     *  Destructor
     */
    virtual ~Value();

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    virtual Value &operator=(const Value &value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(int value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(bool value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(const std::string &value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Value
     */
    Value &operator=(double value);
    
    /**
     *  The type of object
     *  @return Type
     */
    Type type();
    
    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    void setType(Type type);

    /**
     *  Is this a NULL value?
     *  @return bool
     */
    bool isNull();

    /**
     *  Is this an integer value?
     *  @return bool
     */
    bool isInt();
    
    /**
     *  Is this a boolean value?
     *  @return bool
     */
    bool isBool();
    
    /**
     *  Is this a string value?
     *  @return bool
     */
    bool isString();
    
    /**
     *  Is this a decimal value?
     *  @return bool
     */
    bool isDecimal();
    
    /**
     *  Is this an object value?
     *  @return bool
     */
    bool isObject();
    
    /**
     *  Is this an array value?
     *  @return bool
     */
    bool isArray();
    
    /**
     *  Retrieve the value as integer
     *  @return int
     */
    int intValue();
    
    /**
     *  Retrieve the value as boolean
     *  @return bool
     */
    bool boolValue();
    
    /**
     *  Retrieve the value as string
     *  @return string
     */
    std::string stringValue();
    
    /**
     *  Retrieve the value as decimal
     *  @return double
     */
    double decimalValue();
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int size();
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int count()
    {
        return size();
    }
    
    /**
     *  The number of members in case of an array or object
     *  @return int
     */
    int length()
    {
        return size();
    }
    
    /**
     *  Cast to an int
     *  @return int
     */
    operator int ()
    {
        return intValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool ()
    {
        return boolValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string ()
    {
        return stringValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double ()
    {
        return decimalValue();
    }
    
    /**
     *  Array access operator
     *  This can be used for accessing arrays
     *  Be aware: if the 'this' object is not already an array, it will be converted into one!
     *  @param  index
     *  @return Value
     */
    Value operator[](int index);
    
    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  Be aware: if the 'this' object is not already an array, it will be converted into one!
     *  @param  key
     *  @return Value
     */
    Value operator[](const std::string &key);

    /**
     *  Array access operator
     *  This can be used for adding a record to the array
     *  Be aware: if the 'this' object is not already an array, it will be converted into one!
     *  @param  key
     *  @return Value
     */
    //Value operator[]();


protected:
    /**
     *  The wrapped zval
     *  @var struct zval
     */
    struct _zval_struct *_val;
};

/**
 *  End of namespace
 */
}


