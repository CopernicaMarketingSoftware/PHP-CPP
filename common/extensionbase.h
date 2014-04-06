/**
 *  ExtensionBase.h
 *
 *  Base class for ExtensionImpl objects. Common code used by both the Zend
 *  and HHVM engine.
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
class ExtensionBase
{
protected:
    /**
     *  Pointer to the extension object that is filled by the extension programmer
     *  @var    Extension
     */
    Extension *_data;

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
     */
    ExtensionBase(Extension *data) : _data(data) {}
    
    /**
     *  No copy'ing and no moving
     */
    ExtensionBase(const ExtensionImpl &extension) = delete;
    ExtensionBase(ExtensionImpl &&extension) = delete;
    
    /**
     *  Destructor
     */
    virtual ~ExtensionBase() {}
    
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
};

/**
 *  End of namespace
 */
}

