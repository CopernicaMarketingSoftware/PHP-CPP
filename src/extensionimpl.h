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
class ExtensionImpl
{
protected:
    /**
     *  Pointer to the extension object that is filled by the extension programmer
     *  @var    Extension
     */
    Extension *_data;

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
     *  Callback that is called after the engine is initialized and before the
     *  pageviews are going to be handled
     *  @var    Callback
     */
    Callback _onStartup;
    
    /**
     *  Callback that is called in front of each request
     *  @var    Callback
     */
    Callback _onRequest;
    
    /**
     *  Callback that is called right after each request
     *  @var    Callback
     */
    Callback _onIdle;
    
    /**
     *  Callback that is called right before the engine is closing down
     *  @var    Callback
     */
    Callback _onShutdown;
    
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
     *  Register a function to be called when the PHP engine is ready
     * 
     *  The callback will be called after all extensions are loaded, and all 
     *  functions and classes are available, but before the first pageview/request
     *  is handled. You can register this callback if you want to be notified
     *  when the engine is ready, for example to initialize certain things.
     * 
     *  @param  callback
     */
    void onStartup(const Callback &callback)
    {
        // copy callback
        _onStartup = callback;
    }
    
    /**
     *  Register a function to be called when the PHP engine is going to stop
     * 
     *  The callback will be called right before the process is going to stop.
     *  You can register a function if you want to clean up certain things.
     * 
     *  @param  callback
     */
    void onShutdown(const Callback &callback)
    {
        // copy callback
        _onShutdown = callback;
    }
    
    /**
     *  Register a callback that is called at the beginning of each pageview/request
     * 
     *  You can register a callback if you want to initialize certain things
     *  at the beginning of each request. Remember that the extension can handle
     *  multiple requests after each other, and you may want to set back certain
     *  global variables to their initial variables in front of each request
     * 
     *  @param  callback
     */
    void onRequest(const Callback &callback)
    {
        // copy callback
        _onRequest = callback;
    }
    
    /**
     *  Register a callback that is called to cleanup things after a pageview/request
     * 
     *  The callback will be called after _each_ request, so that you can clean up
     *  certain things and make your extension ready to handle the next request.
     *  This method is called onIdle because the extension is idle in between
     *  requests.
     * 
     *  @param  callback
     */
    void onIdle(const Callback &callback)
    {
        // copy callback
        _onIdle = callback;
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
    static int onStartup(int type, int module_number TSRMLS_DC);
    
    /**
     *  Function that is called when the extension is about to be stopped
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int
     */
    static int onShutdown(int type, int module_number TSRMLS_DC);
    
    /**
     *  Function that is called when a request starts
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int onRequest(int type, int module_number TSRMLS_DC);

    /**
     *  Function that is called when a request is ended
     *  @param  type        Module type
     *  @param  number      Module number
     *  @param  tsrm_ls
     *  @return int         0 on success
     */
    static int onIdle(int type, int module_number TSRMLS_DC);
};

/**
 *  End of namespace
 */
}


