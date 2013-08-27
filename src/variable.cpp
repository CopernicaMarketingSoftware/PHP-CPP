/**
 *  Variable.cpp
 *
 *  Implementation for the Variable class, which wraps a PHP userspace 
 *  variable (a 'zval' in Zend's terminology) into a C++ object
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
Variable::Variable()
{
    // create a null zval
    MAKE_STD_ZVAL(_val);
    ZVAL_NULL(_val);
}

/**
 *  Constructor based on integer value
 *  @param  value
 */
Variable::Variable(int value)
{
    // create an integer zval
    MAKE_STD_ZVAL(_val);
    ZVAL_LONG(_val, value);
}

/**
 *  Constructor based on boolean value
 *  @param  value
 */
Variable::Variable(bool value)
{
    // create a boolean zval
    MAKE_STD_ZVAL(_val);
    ZVAL_BOOL(_val, value);
}

/**
 *  Constructor based on string value
 *  @param  value
 */
Variable::Variable(const std::string &value)
{
    // create a string zval
    MAKE_STD_ZVAL(_val);
    ZVAL_STRINGL(_val, value.c_str(), value.size(), 1);
}

/**
 *  Constructor based on decimal value
 *  @param  value
 */
Variable::Variable(double value)
{
    // create a double zval
    MAKE_STD_ZVAL(_val);
    ZVAL_DOUBLE(_val, value);
}

/**
 *  Wrap object around zval
 *  @param  zval
 */
Variable::Variable(struct _zval_struct *zval)
{
    // just copy the zval into this object
    _val = zval;
}

/**
 *  Copy constructor
 *  @param  value
 */
Variable::Variable(const Variable &that)
{
    // @todo implementation, what should we do 
    
    
}

/**
 *  Destructor
 */
Variable::~Variable()
{
    // @todo implementation
    
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Variable
 */
Variable &Variable::operator=(const Variable &value)
{
    // skip self assignment
    if (this == &value) return *this;
    
    // @todo implementation
    

    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Variable
 */
Variable &Variable::operator=(int value)
{
    // @todo implementation
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Variable
 */
Variable &Variable::operator=(bool value)
{
    // @todo implementation
    
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Variable
 */
Variable &Variable::operator=(const std::string &value)
{
    // @todo implementation
    
    
    // done
    return *this;
}

/**
 *  Assignment operator
 *  @param  value
 *  @return Variable
 */
Variable &Variable::operator=(double value)
{
    // @todo implementation
    
    
    // done
    return *this;
}

/**
 *  The type of object
 *  @return Type
 */
Type Variable::type()
{
    return (Type)Z_TYPE_P(_val);
}

/**
 *  Change the internal type
 *  @param  type
 */
void Variable::setType(Type type)
{
    // @todo implementation
    
}

/**
 *  Is this a NULL value?
 *  @return bool
 */
bool Variable::isNull()
{
    return Z_TYPE_P(_val) == IS_NULL;
}

/**
 *  Is this an integer value?
 *  @return bool
 */
bool Variable::isInt()
{
    return Z_TYPE_P(_val) == IS_LONG;
}

/**
 *  Is this a boolean value?
 *  @return bool
 */
bool Variable::isBool()
{
    return Z_TYPE_P(_val) == IS_BOOL;
}

/**
 *  Is this a string value?
 *  @return bool
 */
bool Variable::isString()
{
    return Z_TYPE_P(_val) == IS_STRING;
}

/**
 *  Is this a decimal value?
 *  @return bool
 */
bool Variable::isDecimal()
{
    return Z_TYPE_P(_val) == IS_DOUBLE;
}

/**
 *  Is this an object value?
 *  @return bool
 */
bool Variable::isObject()
{
    return Z_TYPE_P(_val) == IS_OBJECT;
}

/**
 *  Is this an array value?
 *  @return bool
 */
bool Variable::isArray()
{
    return Z_TYPE_P(_val) == IS_ARRAY;
}

/**
 *  Retrieve the value as integer
 *  @return int
 */
int Variable::intValue()
{
    // @todo implementation
}

/**
 *  Retrieve the value as boolean
 *  @return bool
 */
bool Variable::boolValue()
{
    // @todo implementation
}

/**
 *  Retrieve the value as string
 *  @return string
 */
std::string Variable::stringValue()
{
    // @todo implementation
}

/**
 *  End of namespace
 */
}

