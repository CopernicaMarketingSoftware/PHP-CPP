/**
 *  LongMember.h
 *
 *  Implementation for a property that is initially set to a long value
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
class LongMember : public MemberInfo
{
private:
    /**
     *  The value
     *  @var long
     */
    long _value;
    
public:
    /**
     *  Constructor
     *  @param  value
     */
    LongMember(long value) : MemberInfo(), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~LongMember() {}

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
        zend_declare_property_long(entry, name, size, _value, flags);
    }
};

/**
 *  End of namespace
 */
}

