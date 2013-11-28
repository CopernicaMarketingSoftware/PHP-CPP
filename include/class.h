/**
 *  Class.h
 *
 *  When a class is registered in the extension, you need this helper class
 *  for that.
 *
 *  The use of it is simple:
 *
 *  Extension::add(Class<YourClass>);
 *
 *  Note that YourClass must extend from Php::Object
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_class_entry;

/**
 *  Set up namespace
 */
namespace Php {
 
/**
 *  Class definition of the class
 */
template <typename T>
class Class
{
public:
    /**
     *  Constructor
     */
    Class() {}
    
    /**
     *  Constructor with initializer list to define the properties
     *  @param  members
     */
    Class(const std::initializer_list<Member> &members) : _members(members) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Class(Class &&that) : _members(std::move(that._members)) {}

    /**
     *  Copy constructor
     */
    Class(const Class &that) : _members(that._members) {}
    
    /**
     *  Destructor
     */
    virtual ~Class() {}
    
    /**
     *  Construct an instance
     *  @return Base
     */
    Base* construct()
    {
        // allocate the object
        return new T();
    }

    /**
     *  Initialize the class
     *  This will declare all members
     *  @param entry
     */
    void initialize(struct _zend_class_entry *entry)
    {
        // loop through the members
        for (auto iter = _members.begin(); iter != _members.end(); iter++)
        {
            iter->declare(entry);
        }
    }
    
    /**
     *  Retrieve the functions
     *  @param  classname
     *  @return zend_function_entry*
     */
    struct _zend_function_entry *methods(const char *classname)
    {
        return _members.methods(classname);
    }

protected:
    /**
     *  The initial arguments
     *  @var Members
     */
    Members _members;

};

/**
 *  End of namespace
 */
}

