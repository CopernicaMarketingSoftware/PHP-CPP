/**
 *  NullMember.h
 *
 *  Implementation for a property that is initially set to NULL
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
class NullMember : public MemberInfo
{
public:
    /**
     *  Constructor
     */
    NullMember() : MemberInfo() {}

    /**
     *  Destructor
     */
    virtual ~NullMember() {}

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
        if (flags == constMember) zend_declare_property_null(entry, name, size, flags);
        else zend_declare_class_constant_null(entry, name, size);
#else
        if (flags == constMember) zend_declare_property_null(entry, (char *) name, size, flags);
        else zend_declare_class_constant_null(entry, (char *) name, size);
#endif
    }
};

/**
 *  End of namespace
 */
}

