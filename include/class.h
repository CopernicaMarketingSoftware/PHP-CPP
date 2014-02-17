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
template <typename T, ClassModifier _flags = ClassModifier::regular>
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

    /**
     *  Retrieve the class flags specifying whether the class
     *  is a regular class, abstract or final
     *
     *  @return int flags of access types for classes
     */
    int getFlags()
    {
        return _flags;
    }

protected:
    /**
     *  The initial arguments
     *  @var Members
     */
    Members _members;

};


// C++11 analog of `typedef`. Equivalent to the following pseudocode: typedef ClassFlagged<T, Zend::AccClass::FINAL> FinalClass<T>;
template <typename T>
using FinalClass    = Class<T, ClassModifier::final>;

template <typename T>
using AbstractClass = Class<T, ClassModifier::abstract>;

template <typename T>
using Interface     = Class<T, ClassModifier::interface>;

/**
 *  End of namespace
 */
}

