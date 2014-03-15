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
namespace Php {

/**
 *  If this extension is compiled for a PHP version with multi
 *  threading support, we need an additional header file
 */
#ifdef ZTS
#include "TSRM.h"
#endif

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
 *  The *startup() and *shutdown() callback functions are passed a module_number
 *  variable. However, there does not seem to be a decent API call in Zend to
 *  get back the original module_entry linked to this number. So we have to
 *  look up entries in a hash table to find the right module entry. To make things
 *  even worse, the records in this hash table are copies of the original 
 *  zend_module_entry structure, so we can also not hide the C++ extension
 *  object pointer in the entry that we created ourselves.
 * 
 *  We have an ugly solution, we keep track of a map of all C++ extension names
 *  and their associated extension object, and a map of all module number and
 *  the linked extension object.
 * 
 *  @var map
 */
static std::map<std::string,Extension*> name2extension;
static std::map<int,Extension*> number2extension;

/**
 *  Handler function that is used in combination with zend_hash_apply()
 * 
 *  This function is called when we need to find an extension object based on
 *  an extension number. We loop through the list of all registered modules, and 
 *  for each module we check if we know the extension based on the name
 * 
 *  @param  _zend_module_entry
 */
static int match_module(_zend_module_entry *entry)
{
    // check if there is an extension with this name
    auto iter = name2extension.find(entry->name);
    if (iter == name2extension.end()) return ZEND_HASH_APPLY_KEEP;
    
    // we have the extension, store in combination with the number
    number2extension[entry->module_number] = iter->second;
    
    // done
    return ZEND_HASH_APPLY_KEEP;
}

/**
 *  Find an extension based on the module number
 *  @param  number
 *  @return Extension*
 */
static Extension *find(int number)
{
    // do we already have an extension with this number?
    auto iter = number2extension.find(number);
    if (iter != number2extension.end()) return iter->second;
    
    // no, not yet, loop through all modules
    zend_hash_apply(&module_registry, (apply_func_t)match_module);
    
    // find again
    iter = number2extension.find(number);
    if (iter == number2extension.end()) return nullptr;
    
    // found!
    return iter->second;
}

/**
 *  Function that is called when the extension initializes
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
int Extension::onStartup(int type, int module_number)
{
    // initialize and allocate the "global" variables
    ZEND_INIT_MODULE_GLOBALS(phpcpp, init_globals, NULL); 
    
    // get the extension
    Extension *extension = find(module_number);
    
    // initialize namespace
    extension->initialize("");
    
    // is the callback registered?
    if (extension->_onStartup) extension->_onStartup();

    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when the extension is about to be stopped
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int
 */
int Extension::onShutdown(int type, int module_number)
{
    // get the extension
    Extension *extension = find(module_number);
    
    // is the callback registered?
    if (extension->_onShutdown) extension->_onShutdown();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when a request starts
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
int Extension::onRequest(int type, int module_number)
{
    // get the extension
    Extension *extension = find(module_number);
    
    // is the callback registered?
    if (extension->_onRequest) extension->_onRequest();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when a request is ended
 *  @param  type        Module type
 *  @param  number      Module number
 *  @return int         0 on success
 */
int Extension::onIdle(int type, int module_number)
{
    // get the extension
    Extension *extension = find(module_number);
    
    // is the callback registered?
    if (extension->_onIdle) extension->_onIdle();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Constructor
 *  @param  name        Name of the extension
 *  @param  version     Version number
 *  @param  start       Request start callback
 *  @param  stop        Request stop callback
 */
Extension::Extension(const char *name, const char *version) : Namespace("")
{
    // keep extension pointer based on the name
    name2extension[name] = this;
    
    // allocate memory (we allocate this on the heap so that the size of the
    // entry does not have to be defined in the .h file. We pay a performance
    // price for this, but we pay this price becuase the design goal of the
    // PHP-C++ library is to have an interface that is as simple as possible
    _entry = new _zend_module_entry;
    
    // assign all members (apart from the globals)
    _entry->size = sizeof(zend_module_entry);               // size of the data
    _entry->zend_api = ZEND_MODULE_API_NO;                  // api number
    _entry->zend_debug = ZEND_DEBUG;                        // debug mode enabled?
    _entry->zts = USING_ZTS;                                // is thread safety enabled?
    _entry->ini_entry = NULL;                               // the php.ini record
    _entry->deps = NULL;                                    // dependencies on other modules
    _entry->name = name;                                    // extension name
    _entry->functions = NULL;                               // functions supported by this module (none for now)
    _entry->module_startup_func = &Extension::onStartup;    // startup function for the whole extension
    _entry->module_shutdown_func = &Extension::onShutdown;  // shutdown function for the whole extension
    _entry->request_startup_func = &Extension::onRequest;   // startup function per request
    _entry->request_shutdown_func = &Extension::onIdle;     // shutdown function per request
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
    _entry->build_id = (char *)ZEND_MODULE_BUILD_ID;        // check if extension and zend engine are compatible

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
 *  Retrieve the module entry
 *  @return zend_module_entry
 */
zend_module_entry *Extension::module()
{
    // check if functions we're already defined
    if (_entry->functions || _functions.size() == 0) return _entry;

    // allocate memory for the functions
    zend_function_entry *entries = new zend_function_entry[functions() + 1];

    // initialize the entries
    int count = Namespace::initialize("", entries);

    // last entry should be set to all zeros
    zend_function_entry *last = &entries[count];

    // all should be set to zero
    memset(last, 0, sizeof(zend_function_entry));

    // store functions in entry object
    _entry->functions = entries;

    // return the entry
    return _entry;
}

/**
 *  End of namespace
 */
}

