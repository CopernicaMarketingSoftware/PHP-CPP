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
class BoolMember : public Member
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
     *  @param  name
     *  @param  value
     *  @param  flags
     */
    BoolMember(const char *name, bool value, int flags) : Member(name, flags), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~BoolMember() {}

    /**
     *  Virtual method to declare a class constant
     *  @param  entry       Class entry
     */
    virtual void constant(struct _zend_class_entry *entry) override
    {
        zend_declare_class_constant_bool(entry, _name.c_str(), _name.size(), _value);
    }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     */
    virtual void declare(struct _zend_class_entry *entry) override
    {
        // char* cast is necessary for php 5.3
        zend_declare_property_bool(entry, (char *)_name.c_str(), _name.size(), _value, _flags);
    }
};

/**
 *  End of namespace
 */
}

