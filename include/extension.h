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
 *  Set up namespace
 */
namespace PhpCpp 
{

/**
 *  Class definition
 */
class Extension
{
public:
    /**
     *  Constructor
     */
    Extension();
    
    /**
     *  Destructor
     */
    virtual ~Extension()
    {
    }
    
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
     *  @return boolean
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
     */
    virtual void finalize()
    {
    }

private:
    
};

/**
 *  End of namespace
 */
}


