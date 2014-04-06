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
class NullMember : public Member
{
public:
    /**
     *  Constructor
     *  @param  name
     *  @param  flags
     */
    NullMember(const char *name, int flags) : Member(name, flags) {}

    /**
     *  Destructor
     */
    virtual ~NullMember() {}

    /**
     *  Internal method to declare the property as constant
     *  @param  zend_class_entry
     *  @param  tsrm_ls
     */
    virtual void constant(struct _zend_class_entry *entry TSRMLS_DC) override
    {
        zend_declare_class_constant_null(entry, _name.c_str(), _name.size() TSRMLS_CC);
    }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  tsrm_ls
     */
    virtual void declare(struct _zend_class_entry *entry TSRMLS_DC) override
    {
        // char* cast is necessary for php 5.3
        zend_declare_property_null(entry, (char *)_name.c_str(), _name.size(), _flags TSRMLS_CC);
    }
};

/**
 *  End of namespace
 */
}

