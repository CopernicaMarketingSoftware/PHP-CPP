/**
 *  Variable.h
 *
 *  Base class for variables that are stored in the Zend engine.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

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
     *  Is this an integer value?
     *  @return bool
     */
    virtual bool isInt();
    
    /**
     *  Is this a boolean value?
     *  @return bool
     */
    virtual bool isBool();
    
    /**
     *  Is this a string value?
     *  @return bool
     */
    virtual bool isString();
    
    /**
     *  Is this a decimal value?
     *  @return bool
     */
    virtual bool isDecimal();
    
    /**
     *  Is this an object value?
     *  @return bool
     */
    virtual bool isObject();
    
    /**
     *  Is this an array value?
     *  @return bool
     */
    virtual bool isArray();
    
    /**
     *  Retrieve the value as integer
     *  @return int
     */
    virtual int intValue();
    
    /**
     *  Retrieve the value as boolean
     *  @return bool
     */
    virtual bool boolValue();
    
    /**
     *  Retrieve the value as string
     *  @return string
     */
    std::string stringValue();
    
    /**
     *  Retrieve the value as decimal
     *  @return double
     */
    virtual double decimalValue();
};

/**
 *  End of namespace
 */
}


