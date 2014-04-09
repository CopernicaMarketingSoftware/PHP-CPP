/**
 *  Argument.h
 *
 *  Class holds information about an argument that is passed to a function.
 *  You'll need this class when you're defining your own functions.
 *
 *  The constructor of the argument is protected. Use the ByVal or ByRef
 *  classes instead.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Argument
{
public:
    /**
     *  Destructor
     */
    virtual ~Argument() {}
    
protected:
    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  type        Argument type
     *  @param  required    Is this argument required?
     *  @param  byref       Is this a reference argument
     */
    Argument(const char *name, Type type, bool required = true, bool byref = false) : 
        _name(name), _type(type), _required(required), _byReference(byref) {}
    
    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  classname   Name of the class
     *  @param  nullable    Can it be null?
     *  @param  required    Is this argument required?
     *  @param  byref       Is this a reference argument?
     */
    Argument(const char *name, const char *classname, bool nullable = true, bool required = true, bool byref = false) :
        _name(name), _type(Type::Object), _classname(classname), _nullable(nullable), _required(required), _byReference(byref) {}
    
public:
    /**
     *  Is this a required argument?
     *  @return bool
     *  @internal
     */
    bool required() const
    {
        return _required;
    }
    
    /**
     *  Name of the argument
     *  @return std::string
     */
    const std::string &name() const
    {
        return _name;
    }
    
    /** 
     *  Type-hint for the argument
     *  @return Type
     */
    Type type() const
    {
        return _type;
    }
    
    /**
     *  If the type is a class, the name of the class
     *  @return std::string
     */
    const std::string &classname() const
    {
        return _classname;
    }
    
    /**
     *  Is it allowed to pass parameter with a null value?
     *  @return bool
     */
    bool allowNull() const
    {
        return _nullable;
    }
    
    /**
     *  Is this a parameter-by-reference?
     *  @return bool
     */
    bool byReference() const
    {
        return _byReference;
    }
    
private:
    /**
     *  Name of the argument
     *  @var std::string
     */
    std::string _name;
    
    /**
     *  Type of argument
     *  @var Type
     */
    Type _type;

    /**
     *  Classname, if this is a parameter that is supposed to be an instance of a class
     *  @var std::string
     */
    std::string _classname;
    
    /**
     *  May the parameter be null?
     *  @var bool
     */
    bool _nullable;

    /**
     *  Is this a required argument
     *  @var    bool
     */
    bool _required;
    
    /**
     *  Is this a 'by-reference' parameter?
     *  @var    bool
     */
    bool _byReference;
};

/**
 *  A list of arguments can be supplied to methods
 *  @type   Arguments
 */
using Arguments = std::initializer_list<Argument>;
    
/**
 *  End of namespace
 */
}

