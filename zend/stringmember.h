/**
 *  StringMember.h
 *
 *  Implementation for a property that is initially set to a string value
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
class StringMember : public Member
{
private:
    /**
     *  The value
     *  @var string
     */
    std::string _value;
    
public:
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     *  @param  size
     *  @param  flags
     */
    StringMember(const char *name, const char *value, size_t size, int flags) : Member(name, flags), _value(value, size) {}

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     *  @param  flags
     */
    StringMember(const char *name, const char *value, int flags) : StringMember(name, value, ::strlen(value), flags) {}

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     *  @param  flags
     */
    StringMember(const char *name, const std::string &value, int flags) : Member(name, flags), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~StringMember() {}

    /**
     *  Virtual method to declare class constant
     *  @param  entry       Class entry
     */
    virtual void constant(struct _zend_class_entry *entry) override
    {
        zend_declare_class_constant_stringl(entry, _name.c_str(), _name.size(), _value.c_str(), _value.size());
    }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     */
    virtual void declare(struct _zend_class_entry *entry) override
    {
        zend_declare_property_stringl(entry, _name.c_str(), _name.size(), (char *)_value.c_str(), _value.size(), _flags);
    }
};

/**
 *  End of namespace
 */
}

