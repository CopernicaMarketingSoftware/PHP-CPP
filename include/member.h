/**
 *  Member.h
 *
 *  Base class for elements of a class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_class_entry;

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
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, const Value &value) : 
        _name(name), _public(pub), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~Member() {}
    
    /**
     *  Internal method to declare the property
     *  @var zend_class_entry
     */
    void declare(struct _zend_class_entry *entry);
    

private:
    /**
     *  Name of the member
     *  @var string
     */
    std::string _name;
    
    /**
     *  Is this a public property
     *  @var bool
     */
    bool _public;
    
    /**
     *  The default value
     *  @var Value
     */
    Value _value;

};
    
/**
 *  End of namespace
 */
}

