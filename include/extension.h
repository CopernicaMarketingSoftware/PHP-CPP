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
 *  This is a template class. You need to pass in the type of an object
 *  that you use for storing request specific state information.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
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
 *  Optional callback types for starting and stopping the request
 *  @param  extension
 */
typedef bool    (*request_callback)(Extension *extension);

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
     *  @param  callback    Function that is called when request starts
     *  @param  callback    Function that is called when request ends
     */
    Extension(const char *name = NULL, const char *version = NULL, request_callback start = NULL, request_callback stop = NULL);
    
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
     *  Initialize the extension after it was registered
     * 
     *  You can override this method to add your own initialization code. The
     *  default implementation registers all classes and namespaces
     * 
     *  @return bool
     */
    virtual bool initialize()
    {
        // initialize the namespace
        Namespace::initialize("");
        
        // ok
        return true;
    }
    
    /**
     *  Finalize the extension after it was registered
     *
     *  You can override this method to do your own cleanup right before the
     *  extension object is going to be destructed
     * 
     *  @return bool
     */
    virtual bool finalize()
    {
        // ok
        return true;
    }

    /**
     *  Start a request
     * 
     *  This method is called when the zend engine is about to start a new
     *  request. Internally, it calls the request() method to instantiate
     *  a new request object, and after that it initializes the request.
     * 
     *  @return boolean
     */
    virtual bool startRequest()
    {
        // ok if no callback was set
        if (!_start) return true;
        
        // call the callback function
        return _start(this);
    }
    
    /**
     *  End a request
     * 
     *  This method is called when the Zend engine is ready with a request.
     *  Internally, it destructs the request
     *
     *  @return boolean
     */
    virtual bool endRequest()
    {
        // ok if no callback is set
        if (!_stop) return true;
        
        // call callback
        return _stop(this);
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
     *  Callback that is called before each request
     *  @var request_callback
     */
    request_callback _start;
    
    /**
     *  Callback that is called after each request
     *  @var request_callback
     */
    request_callback _stop;
    
};

/**
 *  End of namespace
 */
}


