/**
 *  DoubleMember.h
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
class DoubleMember : public MemberInfo
{
private:
    /**
     *  The value
     *  @var double
     */
    double _value;
    
public:
    /**
     *  Constructor
     *  @param  value
     */
    DoubleMember(double value) : MemberInfo(), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~DoubleMember() {}

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
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags) override
    {
#if PHP_VERSION_ID >= 50400
        if (flags == constMember) zend_declare_property_double(entry, name, size, _value, flags);
        else zend_declare_class_constant_double(entry, name, size, _value);
#else
        if (flags == constMember) zend_declare_property_double(entry, (char *)name, size, _value, flags);
        else zend_declare_class_constant_double(entry, (char *)name, size, _value);
#endif
    }
};

/**
 *  End of namespace
 */
}

