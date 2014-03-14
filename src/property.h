/**
 *  Property.h
 *
 *  Internal class for properties that are defined with a getter and setter method
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Property
{
private:
    /**
     *  The getter
     *  @var    getter_callback
     */
    getter_callback _getter = nullptr;
    
    /**
     *  The setter
     *  @var    setter_callback
     */
    setter_callback _setter = nullptr;

public:
    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback &getter, const setter_callback &setter) :
        _getter(getter), _setter(setter) {}
    
    /**
     *  Copy constructor
     *  @param  that
     */
    Property(const Property &that) : 
        _getter(that._getter), _setter(that._setter) {}
    
    /**
     *  Destructor
     */
    virtual ~Property();
    
    /**
     *  Get the property
     *  @param  base        Object to call it on
     *  @return Value
     */
    Value get(Base *base)
    {
        return (base->*_getter)();
    }
    
    /**
     *  Set the property
     *  @param  base        Object to call it on
     *  @param  value       New value
     *  @return bool
     */
    bool set(Base *base, const Value &value)
    {
        if (!_setter) return false;
        (base->*_setter)(value);
        return false;
    }
};    

/**
 *  End of namespace
 */
}

