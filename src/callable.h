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
        _classname(classname), _type(type), _flags(flags)
    {
        // somehow "&this" is not accepted by the compiler, so we make a copy
        Callable *callable = this;

        // append function name to the data (the data contains a pointer
        // to this object, appended with the function name. this is a trick
        // so that we have the pointer to this function available in the 
        // function name by going back a number of bytes)
        _data.reserve(function.size() + sizeof(this));
        _data.assign(std::string((const char *)&callable, sizeof(callable)));
        _data.append(function);
        
        // find the name
        _name = _data.c_str() + sizeof(this);
        
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
    
    /**
     *  Invoke the method
     *  @param  ht      
     *  @param  return_value
     *  @param  return_value_ptr
     *  @param  this_ptr
     *  @param  return_value_used
     *  @param  tsrm_ls
     *  @return integer
     */
    int invoke(INTERNAL_FUNCTION_PARAMETERS);


int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC    

private:
    /**
     *  Classname
     *  @var string
     */
    std::string _classname;
    
    /**
     *  Pointer to current object, appended with function name
     *  @var string
     */
    std::string _data;
    
    /**
     *  Pointer to the function name
     *  @var char*
     */
    const char *_name;
    
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

