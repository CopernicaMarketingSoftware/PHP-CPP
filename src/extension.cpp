/**
 *  Extension.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  If this extension is compiled for a PHP version with multi
 *  threading support, we need an additional header file
 */
#ifdef ZTS
#include "TSRM.h"
#endif

/**
 *  The way how PHP C API deals with "global" variables is stupid.
 * 
 *  This is supposed to turn into a structure that is going to be 
 * 	instantiated for each parallel running request, and for which the 
 * 	PHP engine allocates a certain amount of memory, and a magic
 *  pointer that is passed and should be forwarded to every thinkable 
 * 	PHP function.
 * 
 * 	We don't like this architecture. We have our own request object
 * 	that makes much more sense, and that we use. However, we need
 *  to assign this object somewhere, so that's what we do in this
 *  one and only global variable
 */
ZEND_BEGIN_MODULE_GLOBALS(phpcpp)
    Request *request;
ZEND_END_MODULE_GLOBALS(phpcpp)

/**
 *  And now we're going to define a macro. This also is a ridiculous
 * 	architecture from PHP to get access to a variable from the 
 *  structure above.
 */
#ifdef ZTS
#define REQUEST_G(v) TSRMG(phpcpp_globals_id, zend_phpcpp_globals *, v)
#else
#define REQUEST_G(v) (phpcpp_globals.v)
#endif

/**
 *  We're almost there, we now need to declare an instance of the
 *  structure defined above (if building for a single thread) or some
 *  sort of impossible to understand magic pointer-to-a-pointer (for
 *  multi-threading builds). We make this a static variable because
 *  this already is bad enough.
 */
static ZEND_DECLARE_MODULE_GLOBALS(phpcpp)

/**
 *  Function that must be defined to initialize the "globals"
 *  We do not have to initialize anything, but PHP needs to call this
 *  method (crazy)
 * 	@param	globals
 */
static void php_phpcpp_init_globals(zend_phpcpp_globals *globals) {}

/**
 *  The extension is a sort of singleton, so we keep one pointer to it here
 *  @var Extension
 */
static Extension *extension = nullptr;


/**
 *  Helper method to get back the current extension object
 *  @return	Extension
 */
static Extension *get_extension()
{
	// retrieve the extension or module name (because PHP of course does
	// not pass such extremely useful information as they've no clue how
	// to make a decent API
	zend_module_entry *module = EG(current_module);

	// the pointer to the extension is hidden in front of the name
	return HiddenPointer<Extension>(module->name);
}

/**
 *  Function that is called when the extension initializes
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int extension_startup(INIT_FUNC_ARGS)
{
	
	
	
	// initialize and allocate the "global" variables
//	ZEND_INIT_MODULE_GLOBALS(hello, php_phpcpp_init_globals, NULL);	
	
    // initialize the extension
    return BOOL2SUCCESS(get_extension()->initialize());
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
    return BOOL2SUCCESS(get_extension()->finalize());
}

/**
 *  Function that is called when a request starts
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int request_startup(INIT_FUNC_ARGS)
{
    // create the request
    return BOOL2SUCCESS(get_extension()->startRequest());
}

/**
 *  Function that is called when a request is ended
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int request_shutdown(INIT_FUNC_ARGS)
{
    // end the request
    return BOOL2SUCCESS(get_extension()->endRequest());
}


/**
 *  Constructor
 *  @param  name        Name of the extension
 *  @param  version     Version number
 */
Extension::Extension(const char *name, const char *version, const Functions &functions)
{
	// assign all members (apart from the globals)
	_entry.size = sizeof(zend_module_entry);               // size of the data
	_entry.zend_api = ZEND_MODULE_API_NO;                  // api number
	_entry.zend_debug = ZEND_DEBUG;                        // debug mode enabled?
	_entry.zts = USING_ZTS;                                // is thread safety enabled?
	_entry.ini_entry = NULL;                               // the php.ini record
	_entry.deps = NULL;                                    // dependencies on other modules
	_entry.name = HiddenPointer<Extension>(this, name);    // extension name, with a hidden pointer to the extension object
	_entry.functions = functions.internal();               // functions supported by this module
	_entry.module_startup_func = extension_startup;        // startup function for the whole extension
	_entry.module_shutdown_func = extension_shutdown;      // shutdown function for the whole extension
	_entry.request_startup_func = request_startup;         // startup function per request
	_entry.request_shutdown_func = request_shutdown;       // shutdown function per request
	_entry.info_func = NULL;                               // information for retrieving info
	_entry.version = version;                              // version string
	_entry.globals_size = 0;                               // size of the global variables
	_entry.globals_ptr = NULL;                             // pointer to the globals
	_entry.globals_ctor = NULL;                            // constructor for global variables
	_entry.globals_dtor = NULL;                            // destructor for global variables
	_entry.post_deactivate_func = NULL;                    // unknown function
	_entry.module_started = 0;                             // module is not yet started
	_entry.type = 0;                                       // temporary or persistent module, will be filled by Zend engine
	_entry.handle = NULL;                                  // dlopen() handle, will be filled by Zend engine
	_entry.module_number = 0;                              // module number will be filled in by Zend engine
	_entry.build_id = ZEND_MODULE_BUILD_ID;                // check if extension and zend engine are compatible

	// things that only need to be initialized
#ifdef ZTS
	_entry.globals_id_ptr = NULL;
#else
	_entry.globals_ptr = NULL;
#endif
}

/**
 *  End of namespace
 */
}

