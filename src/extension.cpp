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
 *  Constructor
 *  @param  name        Name of the extension
 *  @param  version     Version number
 */
Extension::Extension(const char *name, const char *version)
{
    // initialize the entry
    _entry.size = sizeof(zend_module_entry);    // size of the data
    _entry.zend_api = ZEND_MODULE_API_NO;       // api number
    _entry.zend_debug = ZEND_DEBUG;             // debug mode enabled?
    _entry.zts = USING_ZTS;                     // is thread safety enabled?
    _entry.ini_entry = NULL;                    // the php.ini record
    _entry.deps = NULL;                         // dependencies on other modules
    _entry.name = name;                         // extension name
    _entry.functions = NULL;                    // functions supported by this module
    _entry.module_startup_func = NULL;          // startup function for the whole extension
    _entry.module_shutdown_func = NULL;         // shutdown function for the whole extension
    _entry.request_startup_func = NULL;         // startup function per request
    _entry.request_shutdown_func = NULL;        // shutdown function per request
    _entry.info_func = NULL;                    // information for retrieving info
    _entry.version = version;                   // version string
    _entry.globals_size = 0;                    // number of global variables
#ifdef ZTS
    _entry.globals_id_ptr = NULL;               // pointer to the globals, thread safe
#else
    _entry.globals_ptr = NULL;                  // pointer to the globals
    _entry.globals_ctor = NULL;                 // constructor for global variables
    _entry.globals_dtor = NULL;                 // destructor for global variables
    _entry.post_deactivate_func = NULL;         // unknown function
    _entry.module_started = 0;                  // module is not yet started
    _entry.type = 0;                            // temporary or persistent module, will be filled by Zend engine
    _entry.handle = NULL;                       // dlopen() handle, will be filled by Zend engine
    _entry.module_number = 0;                   // module number will be filled in by Zend engine
    _entry.build_id = ZEND_MODULE_BUILD_ID;     // check if extension and zend engine are compatible
}

/**
 *  End of namespace
 */
}


