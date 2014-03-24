/**
 *  Member.h
 *
 *  Base class for properties of a class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (public, protected etc)
     */
    Member(const char *name, int flags) : _name(name), _flags(flags) {}
    
    /** 
     *  Destructor
     */
    virtual ~Member() {}

    /**
     *  Initialize the member
     *  @param  zend_class_entry
     *  @param  tsrm_ls
     */
    void initialize(struct _zend_class_entry *entry TSRMLS_DC)
    {
        if (_flags == Const) constant(entry TSRMLS_CC);
        else declare(entry TSRMLS_CC);
    }

protected:
    /**
     *  Internal method to declare the property as constant
     *  @param  zend_class_entry
     *  @param  tsrm_ls
     */
    virtual void constant(struct _zend_class_entry *entry TSRMLS_DC) = 0;
    
    /**
     *  Internal method to declare the property
     *  @param  zend_class_entry
     *  @param  tsrm_ls
     */
    virtual void declare(struct _zend_class_entry *entry TSRMLS_DC) = 0;

protected:
    /**
     *  The member name
     *  @var    std::string
     */
    std::string _name;
    
    /**
     *  The member flags
     *  @var    int
     */
    int _flags;
};

/**
 *  End of namespace
 */
}

