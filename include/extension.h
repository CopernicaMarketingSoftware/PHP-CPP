/**
 *  Extension.h
 *
 *  The extension class is the starting point of your PHP extension. This class
 *  is instantiated the moment the PHP engine starts - for example when the
 *  apache process starts - and will be used for all subsequent requests that
 *  are handled by Apache.
 *
 *  For some environments (for example CLI scripts and FastCGI calls) only one 
 *  request is handled by an extension instance, but for others (when PHP runs
 *  as module in a webserver) many requests are handled by the same extension
 *  instance.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Structures referenced in this class
 */
struct _zend_module_entry;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declaration
 */
class Extension;

/**
 *  Signature of a callback
 */
using Callback = std::function<void()>;

/**
 *  Class definition
 */
class Extension : public Namespace
{
public:
    /**
     *  Constructor that defines a number of functions right away
     *  @param  name        Extension name
     *  @param  version     Extension version string
     */
    Extension(const char *name = NULL, const char *version = NULL);
    
    /**
     *  No copy'ing and no moving
     */
    Extension(const Extension &extension) = delete;
    Extension(Extension &&extension) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Extension();
    
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
    _zend_module_entry *module();
    
    /**
     *  Cast to a module entry
     *  @return _zend_module_entry*
     */
    operator _zend_module_entry * ()
    {
        return module();
    }
    
private:
    /**
     *  The information that is passed to the Zend engine
     * 
     *  Although it would be slightly faster to not make this a pointer, this
     *  would require that client code also includes the PHP header files, which
     *  we try to prevent with the PHP-CPP library, so we allocate it dynamically.
     * 
     *  @var zend_module_entry
     */
    _zend_module_entry *_entry;
    
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

    /**
     *  Function that is called when the extension initializes
     *  @param  type        Module type
     *  @param  number      Module number
     *  @return int         0 on success
     */
    static int onStartup(int type, int module_number);

    /**
     *  Function that is called when the extension is about to be stopped
     *  @param  type        Module type
     *  @param  number      Module number
     *  @return int
     */
    static int onShutdown(int type, int module_number);
    
        /**
    *  Function that is called when a request starts
    *  @param  type        Module type
    *  @param  number      Module number
    *  @return int         0 on success
    */
    static int onRequest(int type, int module_number);

    /**
    *  Function that is called when a request is ended
    *  @param  type        Module type
    *  @param  number      Module number
    *  @return int         0 on success
    */
    static int onIdle(int type, int module_number);
};

/**
 *  End of namespace
 */
}


