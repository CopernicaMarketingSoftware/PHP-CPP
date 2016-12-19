/**
 *  FloatMember.h
 *
 *  Implementation for a property that is initially set to a boolean value
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
class FloatMember : public Member
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
     *  @param  name
     *  @param  value
     *  @param  flags
     */
    FloatMember(const char *name, double value, int flags) : Member(name, flags), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~FloatMember() {}

    /**
     *  Virtual method to declare class constant
     *  @param  entry       Class entry
     */
    virtual void constant(struct _zend_class_entry *entry) override
    {
        zend_declare_class_constant_double(entry, _name.c_str(), _name.size(), _value);
    }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry'
     */
    virtual void declare(struct _zend_class_entry *entry) override
    {
        // converstion to char* necessary for php 5.3
        zend_declare_property_double(entry, (char *)_name.c_str(), _name.size(), _value, _flags);
    }
};

/**
 *  End of namespace
 */
}

