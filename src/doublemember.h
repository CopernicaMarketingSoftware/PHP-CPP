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
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)
    {
        flags = Flag(AccProp::PROTECTED);
#if PHP_VERSION_ID >= 50400
        zend_declare_property_double(entry, name, size, _value, flags);
        //zend_update_static_property_double(entry, name, size, _value);
#else
        zend_declare_property_double(entry, (char *)name, size, _value, flags);
#endif
    }


    /**
     *  Virtual method to declare the class constant
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declareConst(struct _zend_class_entry *entry, const char *name, int size)
    {
#if PHP_VERSION_ID >= 50400
        zend_declare_class_constant_double(entry, name, size, _value);
        std::cout << "zend_declare_class_constant_double(" << name << ", " << _value << ")" << std::endl;
#else
        zend_declare_class_constant_double(entry, (char *)name, size, _value);
#endif
    }

};

/**
 *  End of namespace
 */
}

