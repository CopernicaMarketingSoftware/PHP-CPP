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
 *  Class definition
 */
class Parameters : public std::vector<Value>
{
public:
    /**
     *  Constructor
     *  @param  argc    Number of arguments
     *  @param  tsrm_ls
     */
    Parameters(int argc);// TSRMLS_DC);

    /**
     *  Destructor
     */
    virtual ~Parameters() {}
};

/**
 *  End of namespace
 */
}

