/**
 *  CompilerOptions.h
 *
 *  Helper class to temporarily set compiler options
 * 
 *  When an object is destructed, it automatically restored the previous compiler settings
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Class definition
 */
class CompilerOptions
{
private:
    /**
     *  The original compiler options
     *  @var int
     */
    zend_uint _original;
    
#ifdef ZTS
    /**
     *  When in thread safety mode, we also keep track of the TSRM_LS var
     *  @var void***
     */
    void ***tsrm_ls;
#endif

public:
    /**
     *  Constructor
     *  @param  options
     */
    CompilerOptions(zend_uint options TSRMLS_DC)
    {
        // remember the old compiler options before we set temporary compile options
        _original = CG(compiler_options);
        
        // we're going to evaluate only once
        CG(compiler_options) = options;
        
#ifdef ZTS
        // copy tsrm_ls param
        this->tsrm_ls = tsrm_ls;
#endif
    }
    
    /**
     *  Destructor
     */
    virtual ~CompilerOptions()
    {
        // restore original options
        CG(compiler_options) = _original;
    }
};

/**
 *  End of namespace
 */
}

