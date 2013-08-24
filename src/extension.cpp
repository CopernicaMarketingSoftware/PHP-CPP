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
 *  Pointer to the one and only extension
 *  @var Extension
 */
static Extension *extension;

/**
 *  Constructor
 *  @param  name        Name of the extension
 *  @param  version     Version number
 */
Extension::Extension(const char *name, const char *version) : _name(name), _version(version), _entry(NULL), _request(NULL)
{
    // store pointer to the one and only extension
    extension = this;
}

/**
 *  Destructor
 */
Extension::~Extension()
{
    // deallocate entry
    if (_entry) delete _entry;
}

/**
 *  Function that is called when the extension initializes
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
static int extension_startup(INIT_FUNC_ARGS)
{
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
    // create the request
    return BOOL2SUCCESS(extension->startRequest());
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
    return BOOL2SUCCESS(extension->endRequest());
}

/**
 *  Retrieve a pointer to the entry
 *  @return zend_module_entry
 */
zend_module_entry *Extension::entry()
{
    // already initialized?
    if (_entry) return _entry;
    
    // allocate now
    _entry = new zend_module_entry;
    
    // assign all members
    _entry->size = sizeof(zend_module_entry);               // size of the data
    _entry->zend_api = ZEND_MODULE_API_NO;                  // api number
    _entry->zend_debug = ZEND_DEBUG;                        // debug mode enabled?
    _entry->zts = USING_ZTS;                                // is thread safety enabled?
    _entry->ini_entry = NULL;                               // the php.ini record
    _entry->deps = NULL;                                    // dependencies on other modules
    _entry->name = _name;                                   // extension name
    _entry->functions = NULL;                               // functions supported by this module
    _entry->module_startup_func = extension_startup;        // startup function for the whole extension
    _entry->module_shutdown_func = extension_shutdown;      // shutdown function for the whole extension
    _entry->request_startup_func = request_startup;         // startup function per request
    _entry->request_shutdown_func = request_shutdown;       // shutdown function per request
    _entry->info_func = NULL;                               // information for retrieving info
    _entry->version = _version;                             // version string
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
    
    // done
    return _entry;
}

/**
 *  End of namespace
 */
}


