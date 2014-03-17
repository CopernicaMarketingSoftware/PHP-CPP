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
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward declaration
 */
struct _zend_arg_info;

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
     *  Copy constructor
     *  @param  argument
     */
    Argument(const Argument &argument);

    /**
     *  Move constructor
     *  @param  argument
     */
    Argument(Argument &&argument);

    /**
     *  Destructor
     */
    virtual ~Argument();
    
protected:
    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  type        Argument type
     *  @param  required    Is this argument required?
     *  @param  byref       Is this a reference argument
     */
    Argument(const char *name, Type type, bool required = true, bool byref = false);
    
    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  classname   Name of the class
     *  @param  nullable    Can it be null?
     *  @param  required    Is this argument required?
     *  @param  byref       Is this a reference argument?
     */
    Argument(const char *name, const char *classname, bool nullable = true, bool required = true, bool byref = false);
    
public:
    /**
     *  Fill an arg_info structure with data
     *  @param  info
     *  @internal
     */
    void fill(struct _zend_arg_info *info) const;
    
    /**
     *  Is this a required argument?
     *  @return bool
     *  @internal
     */
    bool required() const
    {
        return _required;
    }

private:
    /**
     *  The argument info
     *  @var    zend_arg_info
     */
    struct _zend_arg_info *_info;
    
    /**
     *  Is this a required argument
     *  @var    bool
     */
    bool _required;
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

