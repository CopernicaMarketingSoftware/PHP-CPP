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
 */

/**
 *  Forward declarations
 */
struct _zend_module_entry;

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Class definition
 */
class Extension
{
public:
    /**
     *  Constructor
     *  @param  name        Extension name
     *  @param  version     EXtension version
     */
    Extension(const char *name, const char *version);
    
    /**
     *  Destructor
     */
    virtual ~Extension();
    
    /**
     *  Initialize the extension.
     *  
     *  This method is called after the extension has been loaded, constructed 
     *  and after the compatibility has been checked, but before the requests 
     *  are handled. You can override this method to add your own initialization.
     * 
     *  The method should return true on success, and false on failure (in which
     *  case the extension will not be used)
     * 
     *  @return bool
     */
    virtual bool initialize()
    {
        return true;
    }
    
    /**
     *  Finalize the extension
     *  
     *  This method gets called after all requests were handled, and right before 
     *  the Apache module or CLI script will exit. You can override it to add
     *  your own cleanup code.
     * 
     *  @return bool
     */
    virtual bool finalize()
    {
        return true;
    }
    
    /**
     *  Internal method to get access to the entry
     *  @return zend_module_entry
     */
    _zend_module_entry *entry();
    
private:
    /**
     *  Extension name
     *  @var char*
     */
    const char *_name;
    
    /**
     *  Extension version
     *  @var char*
     */
    const char *_version;

    /**
     *  The information that is passed to the Zend engine
     *  @var zend_module_entry
     */
    _zend_module_entry *_entry;



    
};

/**
 *  End of namespace
 */
}


