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
 *  changed by Valeriy Dmitriev <ufabiz@gmail.com>
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
    Class(const std::initializer_list<Member> &members, FlagClass flags = FlagClass(Zend::AccClass::NOSET)) : _members(members), _flags(flags) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Class(Class &&that) : _members(std::move(that._members)), _flags(std::move(that._flags)) {}

    /**
     *  Copy constructor
     */
    Class(const Class &that) : _members(that._members), _flags(that._flags) {}
    
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
    
    /**
     *  Retrieve the functions
     *  @return int flags of access types for classes
     */
    int getFlags() {
        return _flags;
    }

protected:
    /**
     *  The initial arguments
     *  @var Members
     */
    Members _members;

private:
    /**
     *  The access types flags for class
     *  @var Members
     */
    FlagClass _flags;

};

/**
 *  Class definition of the ClassFlagged
 *  template ClassFlagged designed for easy instance of Class<T> for concrete flags
 */
template <typename T, Zend::AccClass Flags>
class ClassFlagged : public Class<T>
{
public:
    ClassFlagged() : Class<T>() {}
    ClassFlagged(const std::initializer_list<Member> &members) : Class<T>(members, FlagClass(Flags)) {}
};

template <typename T>
// C++11 analog of `typedef`. Equivalent to the following pseudocode: typedef ClassFlagged<T, Zend::AccClass::FINAL> FinalClass<T>;
using FinalClass    = ClassFlagged<T, Zend::AccClass::FINAL>;
template <typename T>
using AbstractClass = ClassFlagged<T, Zend::AccClass::ABSTRACT>;
template <typename T>
using Interface     = ClassFlagged<T, Zend::AccClass::INTERFACE>;
template <typename T>
using Trait         = ClassFlagged<T, Zend::AccClass::TRAIT>;

/**
 *  End of namespace
 */
}

