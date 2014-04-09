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
    union {
        getter_callback_0 g0;
        getter_callback_1 g1;
    } _getter;
    
    /**
     *  The setter
     *  @var    setter_callback
     */
    union {
        setter_callback_0 s0;
        setter_callback_1 s1;
    } _setter;
    
    /**
     *  Type of getter
     *  @var char
     */
    int _gtype = 0;
    
    /**
     *  Type of setter
     *  @var char
     */
    int _stype = 100;

public:
    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_0 &getter) : _gtype(0)
    {
        _getter.g0 = getter;
    }

    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_1 &getter) : _gtype(1)
    {
        _getter.g1 = getter;
    }


    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_0 &getter, const setter_callback_0 &setter) : _gtype(0), _stype(0)
    {
        _getter.g0 = getter;
        _setter.s0 = setter;
    }

    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_1 &getter, const setter_callback_0 &setter) : _gtype(1), _stype(0)
    {
        _getter.g1 = getter;
        _setter.s0 = setter;
    }

    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_0 &getter, const setter_callback_1 &setter) : _gtype(0), _stype(1)
    {
        _getter.g0 = getter;
        _setter.s1 = setter;
    }

    /**
     *  Constructor
     *  @param  getter
     *  @param  setter
     */
    Property(const getter_callback_1 &getter, const setter_callback_1 &setter) : _gtype(1), _stype(1)
    {
        _getter.g1 = getter;
        _setter.s1 = setter;
    }

    /**
     *  Copy constructor
     *  @param  that
     */
    Property(const Property &that) : 
        _getter(that._getter), _setter(that._setter), _gtype(that._gtype), _stype(that._stype) {}
    
    /**
     *  Destructor
     */
    virtual ~Property() {}
    
    /**
     *  Get the property
     *  @param  base        Object to call it on
     *  @return Value
     */
    Value get(Base *base)
    {
        if (_gtype == 0) return (base->*_getter.g0)();
        else return (base->*_getter.g1)();
    }
    
    /**
     *  Set the property
     *  @param  base        Object to call it on
     *  @param  value       New value
     *  @return bool
     */
    bool set(Base *base, const Value &value)
    {
        switch (_stype) {
        case 0: (base->*_setter.s0)(value); return true;
        case 1: (base->*_setter.s1)(value); return true;
        default: return false;
        }
    }
};    

/**
 *  End of namespace
 */
}

