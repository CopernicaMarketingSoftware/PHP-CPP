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
     *  @var zend_module_entry
     */
    zend_module_entry _entry;
    
    /**
     *  Is the object locked? This prevents crashes for 'apache reload'
     *  because we then do not have to re-initialize the entire php engine
     *  @var bool
     */
    bool _locked = false;
    
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
     *  @param  apiversion  API version number
     */
    ExtensionImpl(Extension *data, const char *name, const char *version, int apiversion);
    
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
     *  The extension name
     *  @return const char *
     */
    const char *name() const;
    
    /**
     *  The extension version
     *  @return const char *
     */
    const char *version() const;
    
    /** 
     *  Is the object locked (true) or is it still possible to add more functions,
     *  classes and other elements to it?
     *  @return bool
     */
    bool locked()
    {
        // return member
        return _locked;
    }
    
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
     *  @param  module_number
     *  @param  tsrm_ls
     *  @return bool
     */
    bool initialize(int module_number TSRMLS_DC);

    /**
     *  Shutdown the extension
     *  @param  module_number
     *  @param  tsrm_ls
     *  @return bool
     */
    bool shutdown(int module_number TSRMLS_DC);

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

    /**
     *  Function that is called when the PHP engine initializes with a different PHP-CPP
     *  version for the libphpcpp.so file than the version the extension was compiled for
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int processMismatch(int type, int module_number TSRMLS_DC);
};

/**
 *  End of namespace
 */
}


