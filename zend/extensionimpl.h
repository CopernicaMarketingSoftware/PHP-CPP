/**
 *  ExtensionImpl.h
 *
 *  Extension implementation for the Zend engine.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ExtensionImpl : public ExtensionBase
{
protected:
    /**
     *  The information that is passed to the Zend engine
     * 
     *  Although it would be slightly faster to not make this a pointer, this
     *  would require that client code also includes the PHP header files, which
     *  we try to prevent with the PHP-CPP library, so we allocate it dynamically.
     * 
     *  @var zend_module_entry
     */
    zend_module_entry _entry;
    
    /**
     *  The .ini entries
     *  
     *  @var zend_ini_entry
     */
    zend_ini_entry *_ini = nullptr;
        
public:
    /**
     *  Constructor
     *  @param  data        Extension object created by the extension programmer
     *  @param  name        Name of the extension
     *  @param  version     Version number
     */
    ExtensionImpl(Extension *data, const char *name, const char *version);
    
    /**
     *  No copy'ing and no moving
     */
    ExtensionImpl(const ExtensionImpl &extension) = delete;
    ExtensionImpl(ExtensionImpl &&extension) = delete;
    
    /**
     *  Destructor
     */
    virtual ~ExtensionImpl();
    
    /**
     *  Retrieve the module entry
     * 
     *  This is the memory address that should be exported by the get_module()
     *  function.
     *
     *  @return _zend_module_entry
     */
    zend_module_entry *module();
    
    /**
     *  Cast to a module entry
     *  @return _zend_module_entry*
     */
    operator zend_module_entry * ()
    {
        return module();
    }
    
private:
    /**
     *  Initialize the namespace after it was registered
     *  @param  tsrm_ls
     */
    void initialize(TSRMLS_D);

    /**
     *  Function that is called when the extension initializes
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int processStartup(int type, int module_number TSRMLS_DC);
    
    /**
     *  Function that is called when the extension is about to be stopped
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int
     */
    static int processShutdown(int type, int module_number TSRMLS_DC);
    
    /**
     *  Function that is called when a request starts
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int processRequest(int type, int module_number TSRMLS_DC);

    /**
     *  Function that is called when a request is ended
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int processIdle(int type, int module_number TSRMLS_DC);
};

/**
 *  End of namespace
 */
}


