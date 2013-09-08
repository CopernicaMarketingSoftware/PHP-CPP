/**
 *  Arguments.h
 *
 *  When a function is invoked, it is passed a vector of arguments. This
 *  arguments class, that overrides from vector, takes care of that.
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
class Arguments
{
public:
    /**
     *  Constructor
     *  @param  min     The min number of arguments
     *  @param  max     The max number of arguments
     */
    Arguments(int min, int max);
    
    /**
     *  No copy or move operations
     *  @param  arguments
     */
    Arguments(const Arguments &arguments) = delete;
    Arguments(Arguments &&arguments) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Arguments();
    
    /**
     *  Number of arguments
     *  @return int
     */
    int argc()
    {
        return _max;
    }
    
    /**
     *  Number of required arguments
     *  @return int
     */
    int required()
    {
        return _min;
    }
    
    /**
     *  Get access to internal data
     *  @return zend_arg_info*
     */
    struct _zend_arg_info *internal()
    {
        return _argv;
    }

private:
    /**
     *  Min number of arguments
     *  @var integer
     */
    int _min;
    
    /**
     *  Max number of arguments
     *  @var integer
     */
    int _max;
    
    /**
     *  The arguments
     *  @var zend_arg_info[]
     */
    struct _zend_arg_info *_argv;

};

/**
 *  End of namespace
 */
}

