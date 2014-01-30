/**
 *  MemberInfo.h
 *
 *  Base class for the implementation of class members
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
class MemberInfo
{
private:
    /**
     *  Number of references
     *  @var int
     */
    int _refcount;
    
public:
    /**
     *  Constructor
     */
    MemberInfo() : _refcount(1) {}
    
    /**
     *  Virtual destructor
     */
    virtual ~MemberInfo() {}
    
    /**
     *  Retrieve refcount
     *  @return int
     */
    int refcount() { return _refcount; }
    
    /**
     *  Refcount after making a change
     *  @param  change
     *  @return integer
     */
    int refcount(int change) { return _refcount += change; }
    
    /**
     *  Is this a property member
     *  @return bool
     */
    virtual bool isProperty() { return false; }
    
    /**
     *  Is this a method member
     *  @return bool
     */
    virtual bool isMethod() { return false; }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags) {};

    /**
     *  Virtual method to declare the constant property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declareConst(struct _zend_class_entry *entry, const char *name, int size) {};
    
    /**
     *  Fill a function entry object
     *  @param  entry       Function entry
     *  @param  classname   Name of the class
     *  @param  flags       Is this a public method?
     */
    virtual void fill(struct _zend_function_entry *entry, const char *classname, int flags) {};
    
};

/**
 *  End of namespace
 */
}


