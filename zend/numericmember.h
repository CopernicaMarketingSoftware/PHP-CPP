/**
 *  NumericMember.h
 *
 *  Implementation for a property that is initially set to a numeric value
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class NumericMember : public Member
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
     *  @param  name
     *  @param  value
     *  @param  flags
     */
    NumericMember(const char *name, long value, int flags) : Member(name, flags), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~NumericMember() {}

    /**
     *  Declare class constant
     *  @param  entry       Class entry
     */
    virtual void constant(struct _zend_class_entry *entry) override
    {
        zend_declare_class_constant_long(entry, _name.c_str(), _name.size(), _value);
    }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     */
    virtual void declare(struct _zend_class_entry *entry) override
    {
        zend_declare_property_long(entry, _name.c_str(), _name.size(), _value, _flags);
    }
};

/**
 *  End of namespace
 */
}

