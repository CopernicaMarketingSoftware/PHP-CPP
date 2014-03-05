/**
 *  Parameters.h
 *
 *  Wrapper around parameters that are passed to a 
 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Forward declarations
 */
class Base;

/**
 *  Class definition
 */
class Parameters : public std::vector<Value>
{
public:
    /**
     *  Constructor
     *  @param  this_ptr    Optional this_ptr
     *  @param  argc        Number of arguments
     *  @param  tsrm_ls
     */
    Parameters(struct _zval_struct *this_ptr, int argc);// TSRMLS_DC);

    /**
     *  Destructor
     */
    virtual ~Parameters() {}
    
    /**
     *  The object that is being called
     *  @return Base
     */
    Base *object();
    
private:
    /**
     *  The this pointer
     *  @var zval
     */
    struct _zval_struct *_this;
};

/**
 *  End of namespace
 */
}

