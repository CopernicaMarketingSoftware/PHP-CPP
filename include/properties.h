/**
 *  Properties.h
 *
 *  The properties of a class are accessible using the protected _properties
 *  member. This is a class that implements the [] operator, so that all 
 *  properties can be accessed using ["name"].
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class properties
 */
class Properties
{
public:
    /**
     *  Destructor
     */
    virtual ~Properties() {}
    
    /**
     *  Get access to a property by name
     *  @param  name
     *  @return HashMember
     */
    HashMember<std::string> operator[](const char *name) 
    { 
        // map to value
        return _value[name];
    }
    
    /**
     *  Another way to get access to a property
     *  @param  name
     *  @return HashMember
     */
    HashMember<std::string> operator[](const std::string &name)
    {
        // map to value
        return _value[name];
    }

private:
    /**
     *  Private constructor - outside users are not supposed to instantiate this object
     *  @param  zval
     */
    Properties(struct _zval_struct *zval) : _value(zval) {}
    
    /**
     *  The value object
     *  @var    Value
     */
    Value _value;
    
    /**
     *  Only the base class can create properties
     */
    friend class Base;
};

/**
 *  End of namespace
 */
}
