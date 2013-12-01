/**
 *  Extension.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

extern "C"
{
	zend_class_entry *default_exception_ce;
}

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  If this extension is compiled for a PHP version with multi
 *  threading support, we need an additional header file
 */
#ifdef ZTS
#include "TSRM.h"
#endif

/**
 *  Pointer to the one and only extension object
 *  @var    Extension
 */
static Extension *extension = nullptr;

/**
 *  We're almost there, we now need to declare an instance of the
 *  structure defined above (if building for a single thread) or some
 *  sort of impossible to understand magic pointer-to-a-pointer (for
 *  multi-threading builds). We make this a static variable because
 *  this already is bad enough.
 */
ZEND_DECLARE_MODULE_GLOBALS(phpcpp)

/**
 *  Function that must be defined to initialize the "globals"
 *  We do not have to initialize anything, but PHP needs to call this
 *  method (crazy)
 *  @param  globals
 */
static void init_globals(zend_phpcpp_globals *globals) {}

/**
 *  Function that is called when the extension initializes
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int extension_startup(INIT_FUNC_ARGS)
{
    // initialize and allocate the "global" variables
    ZEND_INIT_MODULE_GLOBALS(phpcpp, init_globals, NULL); 

    // initialize the extension
    return BOOL2SUCCESS(extension->initialize());
}

/**
 *  Function that is called when the extension is about to be stopped
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int
 */
static int extension_shutdown(SHUTDOWN_FUNC_ARGS)
{
    // finalize the extension
    return BOOL2SUCCESS(extension->finalize());
}

/**
 *  Function that is called when a request starts
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int request_startup(INIT_FUNC_ARGS)
{
    // create the environment
    Environment *environment = extension->createEnvironment();
    
    // store in global structure
    PHPCPP_G(environment) = environment;
    
    // initialize the environment
    environment->initialize();
    
    // start the request
    return BOOL2SUCCESS(environment->initialize() && extension->startRequest(*environment));
}

/**
 *  Function that is called when a request is ended
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int request_shutdown(INIT_FUNC_ARGS)
{
    // retrieve the environment
    Environment *environment = PHPCPP_G(environment);
    
    // end the request
    bool success = extension->endRequest(*environment) && environment->finalize();
    
    // deallocate the environment
    extension->deleteEnvironment(environment);
    
    // reset global variable
    PHPCPP_G(environment) = NULL;
    
    // done
    return BOOL2SUCCESS(success);
}

/**
 *  Constructor
 *  @param  name        Name of the extension
 *  @param  version     Version number
 *  @param  start       Request start callback
 *  @param  stop        Request stop callback
 */
Extension::Extension(const char *name, const char *version, request_callback start, request_callback stop) : _start(start), _stop(stop)
{
    // store extension variable
    extension = this;
    
    // allocate memory (we allocate this on the heap so that the size of the
    // entry does not have to be defined in the .h file. We pay a performance
    // price for this, but we pay this price becuase the design goal of the
    // PHP-C++ library is to have an interface that is as simple as possible
    _entry = new zend_module_entry;
    
    // assign all members (apart from the globals)
    _entry->size = sizeof(zend_module_entry);               // size of the data
    _entry->zend_api = ZEND_MODULE_API_NO;                  // api number
    _entry->zend_debug = ZEND_DEBUG;                        // debug mode enabled?
    _entry->zts = USING_ZTS;                                // is thread safety enabled?
    _entry->ini_entry = NULL;                               // the php.ini record
    _entry->deps = NULL;                                    // dependencies on other modules
    _entry->name = name;                                    // extension name
    _entry->functions = NULL;                               // functions supported by this module (none for now)
    _entry->module_startup_func = extension_startup;        // startup function for the whole extension
    _entry->module_shutdown_func = extension_shutdown;      // shutdown function for the whole extension
    _entry->request_startup_func = request_startup;         // startup function per request
    _entry->request_shutdown_func = request_shutdown;       // shutdown function per request
    _entry->info_func = NULL;                               // information for retrieving info
    _entry->version = version;                              // version string
    _entry->globals_size = 0;                               // size of the global variables
    _entry->globals_ptr = NULL;                             // pointer to the globals
    _entry->globals_ctor = NULL;                            // constructor for global variables
    _entry->globals_dtor = NULL;                            // destructor for global variables
    _entry->post_deactivate_func = NULL;                    // unknown function
    _entry->module_started = 0;                             // module is not yet started
    _entry->type = 0;                                       // temporary or persistent module, will be filled by Zend engine
    _entry->handle = NULL;                                  // dlopen() handle, will be filled by Zend engine
    _entry->module_number = 0;                              // module number will be filled in by Zend engine
    _entry->build_id = ZEND_MODULE_BUILD_ID;                // check if extension and zend engine are compatible

    // things that only need to be initialized
#ifdef ZTS
    _entry->globals_id_ptr = NULL;
#else
    _entry->globals_ptr = NULL;
#endif
}

/**
 *  Destructor
 */
Extension::~Extension()
{
    // deallocate functions
    if (_entry->functions) delete[] _entry->functions;
    
    // deallocate entry
    delete _entry;
}

/**
 *  Add a function to the library
 *  @param  function    Function object
 *  @return Function
 */
Function *Extension::add(Function *function)
{
    // add the function to the map
    _functions.insert(std::unique_ptr<Function>(function));
    
    // the result is a pair with an iterator
    return function;
}

/**
 *  Add a native function directly to the extension
 *  @param  name        Name of the function
 *  @param  function    The function to add
 *  @return Function    The added function
 */
Function *Extension::add(const char *name, native_callback_0 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_1 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_2 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_3 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_4 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_5 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_6 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }
Function *Extension::add(const char *name, native_callback_7 function, const std::initializer_list<Argument> &arguments) { return add(new NativeFunction(name, function, arguments)); }

/**
 *  Retrieve the module entry
 *  @return zend_module_entry
 */
zend_module_entry *Extension::module()
{
    // check if functions we're already defined
    if (_entry->functions || _functions.size() == 0) return _entry;

    // allocate memory for the functions
    zend_function_entry *functions = new zend_function_entry[_functions.size() + 1];

    // keep iterator counter
    int i = 0;

    // loop through the functions
    for (auto it = begin(_functions); it != _functions.end(); it++)
    {
        // retrieve entry
        zend_function_entry *entry = &functions[i++];

        // let the function fill the entry
        (*it)->fill(entry);
    }

    // last entry should be set to all zeros
    zend_function_entry *last = &functions[i];

    // all should be set to zero
    memset(last, 0, sizeof(zend_function_entry));

    // store functions in entry object
    _entry->functions = functions;

    // return the entry
    return _entry;
}

/**
 *  Initialize the extension
 *  @return bool
 */
bool Extension::initialize()
{
    // This is a bit of hack. Our Exception class MUST be the first to be registered, because
    // other classes depend on it. However, we can not insert it before the module (extension)
    // is being initialized because we don't have access to certain information from the Zend Engine
    // before the module is initialized.
    GetExceptionBase()->initialize();

    // loop through the classes
    for (auto iter = _classes.begin(); iter != _classes.end(); iter++)
    {
        // initialize the class
        (*iter)->initialize();
    }

    // done
    return true;
}

_ClassInfo * Extension::FindClassByName(const std::string &class_name)
{
	for (auto iter = _classes.begin(); iter != _classes.end(); iter++)
	{
		// initialize the class
		if(std::string((*iter)->_name) == class_name)
			return (*iter).get();
	}

	return NULL;
}

_ClassInfo * Extension::GetExceptionBase()
{
	static _ClassInfo * excp_class = NULL;

	if(excp_class == NULL)
	{
		excp_class = new ClassInfo<Exception>(Php::Constants::ExceptionClassName.c_str(), Php::Class<Exception>(
		{
			Php::Public("__construct", Php::Method<Exception>(&Exception::__construct))
		}));
	}

	return excp_class;
}

/**
 *  End of namespace
 */
}

