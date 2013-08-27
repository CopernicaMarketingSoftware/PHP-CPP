/**
 *  Variable.h
 *
 *  Base class for variables that are stored in the Zend engine.
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
class Variable
{
public:
    /**
     *  Empty constructor (value = NULL)
     */
    Variable();
    
    /**
     *  Constructor based on integer value
     *  @param  value
     */
    Variable(int value);
    
    /**
     *  Constructor based on boolean value
     *  @param  value
     */
    Variable(bool value);
    
    /**
     *  Constructor based on string value
     *  @param  value
     */
    Variable(const std::string &value);
    
    /**
     *  Constructor based on decimal value
     *  @param  value
     */
    Variable(double value);
    
    /**
     *  Wrap object around zval
     *  @param  zval
     */
    Variable(struct _zval_struct *zval);
    
    /**
     *  Copy constructor
     *  @param  value
     */
    Variable(const Variable &that);
    
    /**
     *  Destructor
     */
    virtual ~Variable();

    /**
     *  Assignment operator
     *  @param  value
     *  @return Variable
     */
    virtual Variable &operator=(const Variable &value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Variable
     */
    Variable &operator=(int value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Variable
     */
    Variable &operator=(bool value);

    /**
     *  Assignment operator
     *  @param  value
     *  @return Variable
     */
    Variable &operator=(const std::string &value);
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Variable
     */
    Variable &operator=(double value);
    
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


