/**
 *  Callable.h
 *
 *  This is an internal class that is used internally by the Function and Method 
 *  classes, and that wraps the Zend function entry into a CPP object.
 *
 *  This is an internal class, that is not supposed to be used or called from
 *  outside the PhpCpp library.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Class definition
 */
class Callable
{
public:
    /**
     *  Constructor
     *  @param  classname       Name of the class
     *  @param  function        Name of the function or method
     *  @param  type            Hint for the return type
     *  @param  arguments       The arguments that are passed to the function
     *  @param  flags           Optional flags to be passed to the function
     */
    Callable(const std::string &classname, const std::string &function, Type type = nullType, const std::initializer_list<Argument> &arguments = {}, int flags = 0) :
        _classname(classname), _function(function), _type(type), _flags(flags)
    {
        // process the arguments
        process(arguments);
    }

    /**
     *  Constructor
     *  @param  classname       Name of the class
     *  @param  function        Name of the function or method
     *  @param  type            Hint for the return type
     *  @param  arguments       The arguments that are passed to the function
     *  @param  flags           Optional flags to be passed to the function
     */
    Callable(const std::string &classname, const std::string &function, const std::initializer_list<Argument> &arguments = {}, int flags = 0) : 
        Callable(classname, function, nullType, arguments, flags) {}
    
    /**
     *  Constructor
     *  @param  function        Name of the function or method
     *  @param  type            Hint for the return type
     *  @param  arguments       The arguments that are passed to the function
     *  @param  flags           Optional flags to be passed to the function
     */
    Callable(const std::string &function, Type type = nullType, const std::initializer_list<Argument> &arguments = {}, int flags = 0) :
        Callable("", function, type, arguments, flags) {}

    /**
     *  Constructor
     *  @param  function        Name of the function or method
     *  @param  type            Hint for the return type
     *  @param  arguments       The arguments that are passed to the function
     *  @param  flags           Optional flags to be passed to the function
     */
    Callable(const std::string &function, const std::initializer_list<Argument> &arguments = {}, int flags = 0) :
        Callable("", function, nullType, arguments, flags) {}

    /**
     *  Destructor
     */
    virtual ~Callable()
    {
        delete[] _argv;
    }
    
    /**
     *  Fill a function entry
     *  @param  entry
     */
    void fill(zend_function_entry *entry);
    

private:
    /**
     *  Classname
     *  @var string
     */
    std::string _classname;
    
    /**
     *  Function name
     *  @var string
     */
    std::string _function;
    
    /**
     *  The return type
     *  @var Type
     */
    Type _type;
    
    /**
     *  Function flags (like deprecated, abstract, private, etc)
     *  @var int
     */
    int _flags;
    
    /**
     *  The number of arguments
     *  @var int
     */
    int _argc;
    
    /**
     *  The number of required arguments
     *  @var int
     */
    int _required;
    
    /**
     *  The arguments
     *  @var zend_arg_info[]
     */
    zend_arg_info *_argv;

    /**
     *  Another attempt to fill internal function info
     *  @param  entry
     */
    void fill(zend_internal_function_info *info);
    
    /**
     *  Process the arguments
     *  @param  arguments
     */
    void process(const std::initializer_list<Argument> &arguments);
};

/**
 *  End of namespace
 */
}

