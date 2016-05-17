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
     */
    Callable(const char *name, const Arguments &arguments = {}) :
        _ptr(this, name),
        _argc(arguments.size()),
        _argv(new zend_internal_arg_info[_argc + 1])
    {
        // the first record is initialized with information about the function,
        // so we skip that here
        int i=1;

        // loop through the arguments
        for (auto &argument : arguments)
        {
            // increment counter with number of required parameters
            if (argument.required()) _required++;

            // fill the arg info
            fill(&_argv[i++], argument);
        }
    }

    /**
     *  Copy constructor
     *  @param  that
     */
    Callable(const Callable &that) :
        _ptr(that._ptr),
        _return(that._return),
        _required(that._required),
        _argc(that._argc),
        _argv(nullptr) {}
        // @todo: we have no arguments after copy? is this correct?
        // we do have the argument count though...

    /**
     *  Move constructor
     *  @param  that
     */
    Callable(Callable &&that) :
        _ptr(std::move(that._ptr)),
        _return(that._return),
        _required(that._required),
        _argc(that._argc),
        _argv(std::move(that._argv)) {}

    /**
     *  Destructor
     */
    virtual ~Callable() = default;

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
    void initialize(zend_internal_function_info *info, const char *classname = nullptr) const;


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
     *  @var    unsigned integer
     */
    unsigned int _required = 0;

    /**
     *  Total number of arguments
     *  @var    integer
     */
    int _argc = 0;

    /**
     *  The arguments
     *  @var std::unique_ptr<zend_internal_arg_info[]>
     */
    std::unique_ptr<zend_internal_arg_info[]> _argv;

    /**
     *  Private helper method to fill an argument object
     *  @param  info        object from the zend engine
     *  @param  arg         original object
     */
    void fill(zend_internal_arg_info *info, const Argument &arg) const
    {
        // fill members
        info->name = arg.name();

        // are we filling an object
        if (arg.type() == Type::Object) info->class_name = arg.classname();
        else info->class_name = nullptr;

        // since php 5.4 there is a type-hint, but we only support arrays, objects and callables
        switch (arg.type()) {
        case Type::Array:       info->type_hint = IS_ARRAY; break;
        case Type::Callable:    info->type_hint = IS_CALLABLE; break;
        case Type::Object:      info->type_hint = IS_OBJECT; break;
        default:                info->type_hint = IS_NULL; break;
        }

        // from PHP 5.6 and onwards, an is_variadic property can be set, this
        // specifies whether this argument is the first argument that specifies
        // the type for a variable length list of arguments. For now we only
        // support methods and functions with a fixed number of arguments.
        info->is_variadic = false;

        // this parameter is a regular type
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

};

/**
 *  End of namespace
 */
}

