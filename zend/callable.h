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
     *  @param  return_ref  Return reference or not
     */
    Callable(const char *name, const Arguments &arguments = {}, bool return_ref = false) : _ptr(this, name), _return_ref(return_ref)
    {
        // construct vector for arguments
        _argc = arguments.size();
        _argv = new zend_arg_info[_argc+1];
        
        // the first record is initialized with information about the function,
        // so we skip that here
        int i=1;
        
        // loop through the arguments
        for (auto it = arguments.begin(); it != arguments.end(); it++)
        {
            // increment counter with number of required parameters
            if (it->required()) _required++;
            
            // fill the arg info
            fill(&_argv[i++], *it);
        }
    }
    
    /**
     *  Copy constructor
     *  @param  that
     */
    Callable(const Callable &that) :
        _ptr(that._ptr),
        _return(that._return),
        _return_ref(that._return_ref),
        _required(that._required),
        _argc(that._argc),
        _argv(nullptr) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Callable(Callable &&that) :
        _ptr(std::move(that._ptr)),
        _return(that._return),
        _return_ref(that._return_ref),
        _required(that._required),
        _argc(that._argc),
        _argv(that._argv) 
    {
        // invalidate other object
        that._argv = nullptr;
    }

    /**
     *  Destructor
     */
    virtual ~Callable()
    {
        if (_argv) delete[] _argv;
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
    void initialize(zend_function_entry *entry, const char *classname = nullptr, int flags = 0) const;

    /**
     *  Fill function info
     *  @param  info        Info object to be filled
     *  @param  ns          Active namespace
     *  @param  classname   Optional class name
     */
    void initialize(zend_arg_info *info, const char *classname = nullptr) const;


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
     *  Return reference or value
     *  @var    bool
     */
    bool _return_ref = false;

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
    zend_arg_info *_argv = nullptr;
    
    /**
     *  Private helper method to fill an argument object
     *  @param  info        object from the zend engine
     *  @param  arg         original object
     */
    void fill(zend_arg_info *info, const Argument &arg) const
    {
        // fill members
        info->name = arg.name();
         info->name_len = ::strlen(arg.name());

#if PHP_VERSION_ID >= 50400

        // since php 5.4 there is a type-hint, but we only support arrays, objects and callables
        switch (arg.type()) {
        case Type::Array:       info->type_hint = IS_ARRAY; break;
        case Type::Callable:    info->type_hint = IS_CALLABLE; break;
        case Type::Object:      info->type_hint = IS_OBJECT; break;
        default:                info->type_hint = IS_NULL; break;
        }
        
# if PHP_VERSION_ID >= 50600

        // from PHP 5.6 and onwards, an is_variadic property can be set, this 
        // specifies whether this argument is the first argument that specifies
        // the type for a variable length list of arguments. For now we only
        // support methods and functions with a fixed number of arguments.
        info->is_variadic = false;

# endif

#else

        // php 5.3 code
        info->array_type_hint = arg.type() == Type::Array;
        info->return_reference = false;
        info->required_num_args = 0;   // @todo is this correct?

#endif

        // this parameter is a regular type
        info->class_name = arg.type() == Type::Object ? arg.classname() : nullptr;
        info->class_name_len = arg.type() == Type::Object && arg.classname() ? ::strlen(arg.classname()) : 0;
        info->allow_null = arg.allowNull();
        info->pass_by_reference = arg.byReference();
    }

    /**
     *  Function that is called by the Zend engine every time that a function gets called
     *  @param  ht
     *  @param  return_value
     *  @param  return_value_ptr
     *  @param  this_ptr
     *  @param  return_value_used
     *  @param  tsrm_ls
     *  @return integer
     */
    static void invoke(INTERNAL_FUNCTION_PARAMETERS);

    /**
     *  Function that is called by the Zend engine every time that a function gets called
     *  @param  ht
     *  @param  return_value
     *  @param  return_value_ptr
     *  @param  this_ptr
     *  @param  return_value_used
     *  @param  tsrm_ls
     *  @return integer
     */
    static void invoke_return_ref(INTERNAL_FUNCTION_PARAMETERS);
};

/**
 *  End of namespace
 */
}

