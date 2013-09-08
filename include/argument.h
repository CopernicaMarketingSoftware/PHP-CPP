/**
 *  Argument.h
 *
 *  Class holds information about an argument that is passed to a function.
 *  You'll need this class when you're defining your own functions.
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
     *  Prevent copying
     *  @param  argument
     */
    Argument(const Argument &argument) = delete;
    
    /**
     *  Move constructor
     *  @param  argument
     */
    Argument(Argument &&argument);
    
    /**
     *  Destructor
     */
    virtual ~Argument() {};
    
    /**
     *  Change the name
     *  @param  name
     *  @return Argument
     */
    Argument &name(const char *name);
    
    /**
     *  Change the type
     *  @param  type
     *  @return Argument
     */
    Argument &type(Type type = nullType);
    
    /**
     *  Require the parameter to be a certain class
     *  @param  name        Name of the class
     *  @param  null        Are null values allowed?
     *  @return Argument
     */
    Argument &object(const char *classname, bool null = true);
    
    /**
     *  Is this a by-ref argument?
     *  @param  bool        Mark as by-ref variable
     *  @return Argument
     */
    Argument &byref(bool value = true);
    
    /**
     *  Prevent copy
     *  @param  argument    The argument to copy
     *  @return Argument
     */
    Argument &operator=(const Argument &argument) = delete;

protected:
    /**
     *  Protected constructor, to prevent that users can instantiate the
     *  argument object themselves
     *  @param  info
     */
    Argument(struct _zend_arg_info *info) : _info(info) {}

private:
    /**
     *  The argument info
     *  @var    zend_arg_info
     */
    struct _zend_arg_info *_info;
};
    
/**
 *  End of namespace
 */
}

