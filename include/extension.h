/**
 *  Extension.h
 *
 *  The extension class is the starting point of your PHP extension. This class
 *  is instantiated the moment the PHP engine starts - for example when the
 *  apache process starts - and will be used for all subsequent requests that
 *  are handled by Apache.
 *
 *  For some environments (for example CLI scripts and CGI calls) only one 
 *  request is handled by an extension instance, but for others (when PHP runs
 *  as module in a webserver) many requests are handled by the same extension
 *  instance.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declaration
 */
class ExtensionImpl;

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
    Extension(const char *name, const char *version = "1.0");
    
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
     *  @param  callback    Function to be called
     *  @return Extension   Same object to allow chaining
     */
    Extension &onStartup(const Callback &callback);
    
    /**
     *  Register a function to be called when the PHP engine is going to stop
     * 
     *  The callback will be called right before the process is going to stop.
     *  You can register a function if you want to clean up certain things.
     * 
     *  @param  callback    Function to be called
     *  @return Extension   Same object to allow chaining
     */
    Extension &onShutdown(const Callback &callback);
    
    /**
     *  Register a callback that is called at the beginning of each pageview/request
     * 
     *  You can register a callback if you want to initialize certain things
     *  at the beginning of each request. Remember that the extension can handle
     *  multiple requests after each other, and you may want to set back certain
     *  global variables to their initial variables in front of each request
     * 
     *  @param  callback    Function to be called
     *  @return Extension   Same object to allow chaining
     */
    Extension &onRequest(const Callback &callback);
    
    /**
     *  Register a callback that is called to cleanup things after a pageview/request
     * 
     *  The callback will be called after _each_ request, so that you can clean up
     *  certain things and make your extension ready to handle the next request.
     *  This method is called onIdle because the extension is idle in between
     *  requests.
     * 
     *  @param  callback    Function to be called
     *  @return Extension   Same object to allow chaining
     */
    Extension &onIdle(const Callback &callback);
    
    /**
     *  Retrieve the module pointer
     * 
     *  This is the memory address that should be exported by the get_module()
     *  function.
     *
     *  @return void*
     */
    void *module();
    
    /**
     *  Cast to a module entry
     * 
     *  @return void*
     */
    operator void * ()
    {
        return module();
    }

private:
    /**
     *  The implementation object
     * 
     *  @var ExtensionImpl
     */
    ExtensionImpl *_impl;
};

/**
 *  End of namespace
 */
}


