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
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)=0;
};

/**
 *  End of namespace
 */
}


