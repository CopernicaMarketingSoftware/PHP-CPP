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
     */
    void initialize(struct _zend_class_entry *entry)
    {
        if (_flags == Const) constant(entry);
        else declare(entry);
    }

protected:
    /**
     *  Internal method to declare the property as constant
     *  @param  zend_class_entry
     */
    virtual void constant(struct _zend_class_entry *entry) = 0;
    
    /**
     *  Internal method to declare the property
     *  @param  zend_class_entry
     */
    virtual void declare(struct _zend_class_entry *entry) = 0;

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

