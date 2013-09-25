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
 *  Namespace
 */
namespace Php {

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
     *  Initialize the class
     */
    virtual void initialize() = 0;
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
    ClassInfo(const char *name, const Class<T> &type) : _name(name), _type(type)
    {
    }

    /**
     *  Destructor
     */
    virtual ~ClassInfo() {}

    /**
     *  Initialize the class
     */
    virtual void initialize();

private:
    /**
     *  Class name
     *  @var    string
     */
    std::string _name;
    
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

