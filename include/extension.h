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
 *  A couple of predefined native callback functions that can be registered.
 *  These are functions that optional accept a Request and/or Parameters object,
 *  and that either return void or a Value object. 
 */
typedef void    (*native_callback_0)();
typedef void    (*native_callback_1)(Parameters &);
typedef void    (*native_callback_2)(Request &);
typedef void    (*native_callback_3)(Request &, Parameters &);
typedef Value   (*native_callback_4)();
typedef Value   (*native_callback_5)(Parameters &);
typedef Value   (*native_callback_6)(Request &);
typedef Value   (*native_callback_7)(Request &, Parameters &);

/**
 *  Class definition
 */
class Extension
{
public:
    /**
     *  Constructor that defines a number of functions right away
     *  @param  name        Extension name
     *  @param  version     Extension version string
     *  @param  functions   The functions that are defined
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
     *  Create a new request
     * 
     *  You can override this method if you've created your own request class,
     *  and you'd like to use an instance of that class instead. The returned
     *  object must have been created on the heap.
     * 
     *  @return Request*
     */
    virtual Request *request()
    {
        return new Request(this);
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
    bool startRequest()
    {
//        // failure if we already have a request
//        if (_request) return false;
//        
//        // create the request
//        _request = request();
//    
//        // and initialize it
//        return _request->initialize();

        return true;
    }
    
    /**
     *  End a request
     * 
     *  This method is called when the Zend engine is ready with a request.
     *  Internally, it destructs the request
     *
     *  @return boolean
     */
    bool endRequest()
    {
//        // request must exist
//        if (!_request) return false;
//    
//        // finalize the request
//        bool result = _request->finalize();
//        
//        // destruct the request object
//        delete _request;
//        
//        // done
//        return result;

        return true;
    }
    
    /**
     *  Add a function to the extension
     *  
     *  It is only possible to create functions during the initialization of
     *  the library, before the Extension::module() method is called.
     * 
     *  @param  name        Name of the function
     *  @param  function    The function to add
     *  @return Function    The added function
     */
    Function &add(const char *name, const Function &function);
    
    /**
     *  Add a native function directly to the extension
     *  @param  name        Name of the function
     *  @param  function    The function to add
     *  @return Function    The added function
     */
    Function &add(const char *name, native_callback_0 function);
    Function &add(const char *name, native_callback_1 function);
    Function &add(const char *name, native_callback_2 function);
    Function &add(const char *name, native_callback_3 function);
    Function &add(const char *name, native_callback_4 function);
    Function &add(const char *name, native_callback_5 function);
    Function &add(const char *name, native_callback_6 function);
    Function &add(const char *name, native_callback_7 function);
    
    /**
     *  Retrieve the module entry
     * 
     *  This is the memory address that should be exported by the get_module()
     *  function.
     *
     *  @return _zend_module_entry
     */
    _zend_module_entry *module();
    
    
private:
    /**
     *  Map of function objects defined in the library
     *  @var    map
     */
    std::map<const char *,Function> _functions;

    /**
     *  Hidden pointer to self
     *  @var    HiddenPointer
     */
    HiddenPointer<Extension> _ptr;

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
    
};

/**
 *  End of namespace
 */
}


