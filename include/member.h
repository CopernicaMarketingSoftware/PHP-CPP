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
 *  Forward declarations
 */
class MemberInfo;

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
     */
    Member(const char *name, bool pub);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, std::nullptr_t value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, int value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, long value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, bool value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, char value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, const std::string &value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     *  @param  size        String length
     */
    Member(const char *name, bool pub, const char *value, int size = -1);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  pub         Is this a public property (otherwise it is protected)
     *  @param  value       The value to add
     */
    Member(const char *name, bool pub, double value);

    /**
     *  Copy constructor
     *  @param  member      The member to copy
     */
    Member(const Member &member);
    
    /**
     *  Move constructor
     *  @param  member      The member to move
     */
    Member(Member &&member);

    /**
     *  Destructor
     */
    virtual ~Member();
    
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
     *  The implementation for the member
     *  @var MemberInfo
     */
    MemberInfo *_info;


};
    
/**
 *  End of namespace
 */
}

