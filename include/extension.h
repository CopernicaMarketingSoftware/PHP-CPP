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
     *
     *  The first two parameters should be filled by the extension programmer with the
     *  name of the extension, and the version number of the extension (like "1.0").
     *  The third parameter, apiversion, does not have to be supplied and is best kept
     *  to the default value. This third parameter checks whether the PHP-CPP version
     *  that is currently installed on the server is the same as the PHP-CPP version
     *  that was used to compile the extension with.
     *
     *  @param  name        Extension name
     *  @param  version     Extension version string
     *  @param  apiversion  PHP API version (this should always be PHPCPP_API_VERSION, so you better not supply it)
     */
    Extension(const char *name, const char *version = "1.0", int apiversion = PHPCPP_API_VERSION);
    
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
     *  Add a ini entry to the extension by moving it
     *  @param  ini         The php.ini setting
     *  @return Extension   Same object to allow chaining
     */
    Extension &add(Ini &&ini)
    {
        // skip when locked
        if (locked()) return *this;
        
        // and add it to the list of classes
        _ini_entries.emplace_back(new Ini(std::move(ini)));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a ini entry to the extension by copying it
     *  @param  ini         The php.ini setting
     *  @param  Extension   Same object to allow chaining
     */
    Extension &add(const Ini &ini)
    {
        // skip when locked
        if (locked()) return *this;

        // and add it to the list of classes
        _ini_entries.emplace_back(new Ini(ini));
        
        // allow chaining
        return *this;
    }
    
    /**
     *  Because the add function exists in both the Namespace base class
     *  as well as this extended Extension class, we have to tell the compiler
     *  that the add methods from the base are accessble too
     */
    using Namespace::add;

    /**
     *  The total number of php.ini variables
     *  @return size_t
     */
    size_t iniVariables() const
    {
        return _ini_entries.size();
    }

    /**
     *  Apply a callback to each php.ini variable
     * 
     *  The callback will be called with a reference to the ini variable.
     * 
     *  @param  callback
     */
    void iniVariables(const std::function<void(Ini &ini)> &callback)
    {
        // loop through the entries and apply the callback to each one
        for (auto ini : _ini_entries) callback(*ini);
    }

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
    
protected:
    /**
     *  Is the extension object in a locked state? This happens after the
     *  get_module() function was called for the first time ("apache reload"
     *  forces a new call to get_module())
     * 
     *  @return bool
     */
    virtual bool locked() const override;

private:
    /**
     *  The implementation object
     * 
     *  @var ExtensionImpl
     */
    ExtensionImpl *_impl;

    /**
     *  Ini entry defined by the extension
     *  @var    list
     */
    std::list<std::shared_ptr<Ini>> _ini_entries;

};

/**
 *  End of namespace
 */
}


