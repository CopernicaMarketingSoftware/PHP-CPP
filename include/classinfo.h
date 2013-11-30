/**
 *  ClassInfo.h
 *
 *  Internal class that is constructed by the library and that contains
 *  the information about a class, including its name.
 * 
 *  Users of the PHP-CPP libraries are not supposed to interact with
 *  this class, or instantiate objects of this class.
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
class InternalFunction;

/**
 *  Virtual base class of the classInfo
 * 
 *  We need this virtual base class to store pointers to class objects,
 *  without knowing in advance what sort of object they will hold
 */
class _ClassInfo
{
public:
    /**
     *  Constructor
     *  @param  name
     */
    _ClassInfo(const char *name);
    
    /**
	 *  Constructor
	 *  @param  name
	 *  @param base_class
	 */
	_ClassInfo(const char *name, _ClassInfo *  base_class);

    /**
     *  Destructor
     */
    virtual ~_ClassInfo();

    /**
     *  Initialize the class
     */
    void initialize();
    
    /**
     *  Construct the C++ object
     *  @return Base
     */
    virtual Base *construct() = 0;
    
    /**
     *  Initialize the class
     *  @param  entry
     */
    virtual void initialize(struct _zend_class_entry *entry) = 0;

    /**
     *  Retrieve the methods
     *  @return zend_function_entry[]
     */
    virtual struct _zend_function_entry *methods() = 0;

protected:
    /** 
     *  The class entry
     *  @var    zend_class_entry
     */
    struct _zend_class_entry *_entry;

    /**
     * The class that must serve as the base type.
     */
    _ClassInfo *  _base_type;

    /**
     *  The name
     *  @var    string
     */
    std::string _name;
    
    /**
     *  Constructor function
     *  @var    InternalFunction
     */
    InternalFunction *_constructor;
    
    /**
     *  Destructor function
     *  @var    InternalFunction
     */
    InternalFunction *_destructor;
    
    friend class Extension;
    friend class _Method;
};

/**
 *  Class definition
 */
template <typename T>
class ClassInfo : public _ClassInfo
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the class
     *  @param  type        The class type
     */
    ClassInfo(const char *name, const Class<T> &type) : _ClassInfo(name), _type(type)
    {
    }

    /**
	 *  Constructor
	 *  @param  name        Name of the class
	 *  @param  base_class 	A pointer to a _ClassInfo instance, which must serve as this entry's base class.
	 *  @param  type        The class type
	 */
	ClassInfo(const char *name, _ClassInfo * base_class, const Class<T> &type) : _ClassInfo(name, base_class), _type(type)
	{
	}


    /**
     *  Destructor
     */
    virtual ~ClassInfo() {}

    /**
     *  Construct the object
     *  @return Base
     */
    virtual Base *construct()
    {
        return _type.construct();
    }
    
    /**
     *  Initialize the class
     *  @param entry
     */
    virtual void initialize(struct _zend_class_entry *entry)
    {
        // pass to the entry
        _type.initialize(entry);
    }

    /**
     *  Retrieve the methods
     *  @return zend_function_entry[]
     */
    virtual struct _zend_function_entry *methods()
    {
        // ask class object
        return _type.methods(_name.c_str());
    }

private:
    /**
     *  The class object
     *  @var    Class
     */
    Class<T> _type;
};
    
/**
 *  End of namespace
 */
}

