/**
 *  Callable.h
 *
 *  Object represents a callable function or method that is defined with the CPP 
 *  API.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward definitions
 */
struct _zend_function_entry;
struct _zend_internal_function_info;
 
/**
 *  Set up namespace
 */
namespace Php {

/** 
 *  Class definition
 */
class Callable
{
public:
    /**
     *  Constructor
     *  @param  name        Function or method name
     *  @param  arguments   Information about the arguments
     */
    Callable(const char *name, const Arguments &arguments = {}) : _ptr(this, name)
    {
        // construct vector for arguments
        _argc = arguments.size();
        _argv = new zend_arg_info[_argc+1];
        
        // counter
        int i=1;
        
        // loop through the arguments
        for (auto it = arguments.begin(); it != arguments.end(); it++)
        {
            // fill the arg info
            it->fill(&_argv[i++]);
        }
        
        // @todo find out number of required arguments
        _required = _argc;
    }

    /**
     *  Destructor
     */
    virtual ~Callable()
    {
        delete[] _argv;
    }
    
    /**
     *  Method that gets called every time the function is executed
     *  @param  params      The parameters that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(Parameters &params) = 0;
    
    /**
     *  Fill a function entry
     *  @param  entry       Entry to be filled
     *  @param  ns          Active namespace
     *  @param  classname   Optional class name
     *  @param  flags       Access flags
     */
    void initialize(struct _zend_function_entry *entry, const char *classname = nullptr, int flags = 0) const;

    /**
     *  Fill function info
     *  @param  info        Info object to be filled
     *  @param  ns          Active namespace
     *  @param  classname   Optional class name
     */
    void initialize(struct _zend_internal_function_info *info, const char *classname = nullptr) const;


protected:
    /**
     *  Hidden pointer to the name and the function
     *  @var    HiddenPointer
     */
    HiddenPointer<Callable> _ptr;

    /**
     *  Suggestion for the return type
     *  @var    Type
     */
    Type _return = Type::Null;

    /**
     *  Required number of arguments
     *  @var    integer
     */
    int _required = 0;

    /**
     *  Total number of arguments
     *  @var    integer
     */
    int _argc = 0;

    /**
     *  The arguments
     *  @var zend_arg_info[]
     */
    struct _zend_arg_info *_argv = nullptr;

};

/**
 *  End of namespace
 */
}

