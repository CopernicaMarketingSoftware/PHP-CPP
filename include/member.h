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
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     */
    Member(const char *name, const FlagMemb &&flags);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, std::nullptr_t value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, int value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, long value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, bool value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, char value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, const std::string &value);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     *  @param  size        String length
     */
    Member(const char *name, const FlagMemb &&flags, const char *value, int size = -1);

    /**
     *  Constructor
     *  @param  name        Name of the member
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  value       The value to add
     */
    Member(const char *name, const FlagMemb &&flags, double value);
    
    /**
     *  Constructor
     *  @param  name        Name of the method
     *  @param  flags       Flag access to a class member (bublic, protected etc)
     *  @param  method      The method to add
     */
    Member(const char *name, const FlagMemb &&flags, const _Method &method, const std::initializer_list<Argument> &arguments = {});

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
     *  @param  zend_class_entry
     *  @internal
     */
    void declare(struct _zend_class_entry *entry);
    
    /**
     *  Internal method to fill a function entry
     *  @param  zend_function_entry
     *  @param  classname
     *  @internal
     */
    void fill(struct _zend_function_entry *entry, const char *classname);
    
    /**
     *  Is this a property member
     *  @return bool
     */
    bool isProperty();
    
    /**
     *  Is this a method member
     *  @return bool
     */
    bool isMethod();
    
    /**
     *  Is this a class constant
     *  @return bool
     */
    bool isClassConst();
    
    /**
     *  Is this a class constant
     *  @param  bool _constant
     *  @return bool
     */
    bool isClassConst(bool);
    

private:
    /**
     *  Name of the member
     *  @var string
     */
    std::string _name;
    
    /**
     *  Flag access to a class member (bublic, protected etc)
     *  @var bool
     */
    FlagMemb _accflag;
    
    /**
     *  Is this a constant property
     *  @var bool
     */
    bool _constant;
    
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

