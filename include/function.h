/**
 *  Function.h
 *
 *  Object represents a callable function that is defined with the CPP API.
 *  After you've instantiated the extension, you can add function objects to
 *  it.
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
class Function
{
public:
    /**
     *  Constructor
     *  @param  name    Name of the function
     *  @param  min     Min number of arguments
     *  @param  max     Max number of arguments
     */
    Function(const char *name, const std::initializer_list<Argument> &arguments = {});
    
    /**
     *  No copy and move constructors
     *  @param  function    The other function
     */
    Function(const Function &function) = delete;
    Function(Function &&function) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Function();
    
    /**
     *  No assignment operator
     *  @param  function    The other function
     *  @return Function
     */
    Function &operator=(const Function &function) = delete;
    
    /**
     *  Comparison
     *  @param  function    The other function
     *  @return bool
     */
    bool operator<(const Function &function) const
    {
        return strcmp(name(), function.name()) < 0;
    }

    /**
     *  Comparison
     *  @param  function    The other function
     *  @return bool
     */
    bool operator>(const Function &function) const
    {
        return strcmp(name(), function.name()) > 0;
    }

    /**
     *  Comparison
     *  @param  function    The other function
     *  @return bool
     */
    bool operator==(const Function &function) const
    {
        return strcmp(name(), function.name()) == 0;
    }

    /**
     *  Function name
     *  @return const char *
     */
    const char *name() const
    {
        return _ptr.text();
    }

    /**
     *  Method that gets called every time the function is executed
     *  @param  params      The parameters that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(Parameters &params)
    {
        return nullptr;
    }
    

protected:
    /**
     *  Suggestion for the return type
     *  @var    Type
     */
    Type _type = nullType;

    /**
     *  Required number of arguments
     *  @var    integer
     */
    int _required;

    /**
     *  Total number of arguments
     *  @var    integer
     */
    int _argc;

    /**
     *  The arguments
     *  @var zend_arg_info[]
     */
    struct _zend_arg_info *_argv;

    /**
     *  The object address is stored in a hidden pointer, so that we have access to the function object
     *  @var    HiddenPointer
     */
    HiddenPointer<Function> _ptr;

    /**
     *  Fill a function entry
     *  @param  entry       Entry to be filled
     *  @param  classname   Optional class name
     *  @param  pub         Is this a public property?
     */
    void fill(struct _zend_function_entry *entry, const char *classname=NULL, MemberModifier flags = publicMember) const;

    /**
     *  Fill function info
     *  @param  info        Info object to be filled
     *  @param  classname   Optional class name
     */
    void fill(struct _zend_internal_function_info *info, const char *classname=NULL) const;

    /**
     *  Extension has access to the private members
     */
    friend class Extension;

};

/**
 *  End of namespace
 */
}

