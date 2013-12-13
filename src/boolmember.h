/**
 *  BoolMember.h
 *
 *  Implementation for a property that is initially set to a boolean value
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class BoolMember : public MemberInfo
{
private:
    /**
     *  The value
     *  @var bool
     */
    bool _value;
    
public:
    /**
     *  Constructor
     *  @param  value
     */
    BoolMember(bool value) : MemberInfo(), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~BoolMember() {}

    /**
     *  Is this a property member
     *  @return bool
     */
    virtual bool isProperty() { return true; }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)
    {
#if PHP_VERSION_ID >= 50400      
        zend_declare_property_bool(entry, name, size, _value, flags);
#else
        zend_declare_property_bool(entry, (char *) name, size, _value, flags);
#endif
    }
};

/**
 *  End of namespace
 */
}

