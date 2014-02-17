/**
 *  StringMember.h
 *
 *  Implementation for a property that is initially set to a strnig value
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
class StringMember : public MemberInfo
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
     *  @param  value
     */
    StringMember(const std::string &value) : MemberInfo(), _value(value) {}

    /**
     *  Constructor
     *  @param  value
     *  @param  size
     */
    StringMember(const char *value, int size) : MemberInfo(), _value(value, size) {}

    /**
     *  Destructor
     */
    virtual ~StringMember() {}

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
        if (flags == constMember) zend_declare_class_constant_stringl(entry, name, size, _value.c_str(), _value.size());
        else zend_declare_property_stringl(entry, name, size, _value.c_str(), _value.size(), flags);
#else
        if (flags == constMember) zend_declare_class_constant_stringl(entry, (char*) name, size, (char *) _value.c_str(), _value.size());
        else zend_declare_property_stringl(entry, (char*) name, size, (char *) _value.c_str(), _value.size(), flags);
#endif
    }
};

/**
 *  End of namespace
 */
}

