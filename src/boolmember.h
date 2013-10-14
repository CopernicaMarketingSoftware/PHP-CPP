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
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)
    {
        zend_declare_property_bool(entry, name, size, _value, flags);
    }
};

/**
 *  End of namespace
 */
}

