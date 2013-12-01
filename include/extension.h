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
struct _zend_class_entry;
typedef struct _zend_class_entry zend_class_entry;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Optional callback types for starting and stopping the request
 */
typedef bool    (*request_callback)(Environment &);

/**
 *  A couple of predefined native callback functions that can be registered.
 *  These are functions that optional accept a Request and/or Parameters object,
 *  and that either return void or a Value object. 
 */
typedef void    (*native_callback_0)();
typedef void    (*native_callback_1)(Parameters &);
typedef void    (*native_callback_2)(Environment &);
typedef void    (*native_callback_3)(Environment &, Parameters &);
typedef Value   (*native_callback_4)();
typedef Value   (*native_callback_5)(Parameters &);
typedef Value   (*native_callback_6)(Environment &);
typedef Value   (*native_callback_7)(Environment &, Parameters &);

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
     *  Initialize the extension.
     *  
     *  This method is called after the extension has been loaded, constructed 
     *  and after the compatibility has been checked, but before the requests 
     *  are handled. You can override this method to add your own initialization.
     * 
     *  The default behavior of this function is to enable all classes that are
     *  defined in this extension, so that they are also available in PHP.
     * 
     *  The method should return true on success, and false on failure (in which
     *  case the extension will not be used)
     * 
     *  @return bool
     */
    virtual bool initialize();
    
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
     *  Create a new environment
     * 
     *  You can override this method if you've created your own environment class,
     *  and you'd like to use an instance of that class instead. The returned
     *  object must have been created on the heap.
     * 
     *  @return Environment*
     */
    virtual Environment *createEnvironment()
    {
        // allocate the environment
        return new Environment(this);
    }
    
    /**
     *  Destruct an environment
     *  
     *  This is the counterpart of the createEnvironment method.
     * 
     *  @param  Environment
     */
    virtual void deleteEnvironment(Environment *environment)
    {
        // destruct the environment
        delete environment;
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
    virtual bool startRequest(Environment &environment)
    {
        // ok if no callback was set
        if (!_start) return true;
        
        // call the callback function
        return _start(environment);
    }
    
    /**
     *  End a request
     * 
     *  This method is called when the Zend engine is ready with a request.
     *  Internally, it destructs the request
     *
     *  @return boolean
     */
    virtual bool endRequest(Environment &environment)
    {
        // ok if no callback is set
        if (!_stop) return true;
        
        // call callback
        return _stop(environment);
    }
    
    /**
     *  Add a function to the extension
     *  
     *  It is only possible to create functions during the initialization of
     *  the library, before the Extension::module() method is called.
     * 
     *  Note that the function must have been allocated on the HEAP (using
     *  "new") and that the object will be destructed (using "delete")
     *  by the extension object (you thus do not have to destruct it
     *  yourself!) 
     * 
     *  @param  function    The function to add
     *  @return Function    The added function
     */
    Function *add(Function *function);
    
    /**
     *  Add a native function directly to the extension
     *  @param  name        Name of the function
     *  @param  function    The function to add
     *  @param  arguments   Optional argument specification
     *  @return Function    The added function
     */
    Function *add(const char *name, native_callback_0 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_1 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_2 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_3 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_4 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_5 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_6 function, const std::initializer_list<Argument> &arguments = {});
    Function *add(const char *name, native_callback_7 function, const std::initializer_list<Argument> &arguments = {});

    /**
     *  Add a native class to the extension
     *  @param  name        Name of the class
     *  @param  base_class	A pointer to a _ClassInfo instance that must serve as the base class.
     *  @param  type        The class implementation
     */
    template<typename T>
    ClassInfo<T> * add(const char *name, _ClassInfo *base_class, const Class<T> &type)
    {
        // construct info
    	ClassInfo<T> *info = NULL;
    	if(base_class != NULL)
    		info = new ClassInfo<T>(name, base_class, type);
    	else
    		info = new ClassInfo<T>(name, type);

        // add class
        _classes.insert(std::unique_ptr<_ClassInfo>(info));

        // return the class we just created
        return info;
    }

    /**
	 *  Add a native class to the extension
	 *  @param  name        Name of the class
	 *  @param  type        The class implementation
	 */
	template<typename T>
	void add(const char *name, const Class<T> &type)
	{
		add(name, NULL, type);
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
     * Attempts to find the zend class entry for a class with the specified name.
     * @param class_name The name of the class to find.
     * @returns A pointer to the _zend_class_entry structure for the specified class.
     * 		    If no class with the specified name could be found, NULL will be returned.
     */
    _ClassInfo * FindClassByName(const std::string &class_name);
    
    /**
     * Gets a pointer to the _ClassInfo instance that represents the base class for exception
     * classes.
     * @return A pointer to the _ClassInfo instance that represents the base class for exceptions,
     * 		   NULL is returned when something went wrong.
     */
    _ClassInfo * GetExceptionBase();

private:
    /**
     *  Set of function objects defined in the library
     *  @var    set
     */
    std::set<std::unique_ptr<Function>> _functions;

    /**
     *  Set of classes defined in the library
     *  @var    set
     */
    std::set<std::unique_ptr<_ClassInfo>> _classes;

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


