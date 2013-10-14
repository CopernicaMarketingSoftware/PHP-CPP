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
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)
    {
        zend_declare_property_null(entry, name, size, flags);
    }
};

/**
 *  End of namespace
 */
}

