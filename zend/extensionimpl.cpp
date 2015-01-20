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
static std::map<std::string,ExtensionImpl*> name2extension;
static std::map<int,ExtensionImpl*> number2extension;

/**
 *  Handler function that is used in combination with zend_hash_apply()
 * 
 *  This function is called when we need to find an extension object based on
 *  an extension number. We loop through the list of all registered modules, and 
 *  for each module we check if we know the extension based on the name
 * 
 *  @param  zend_module_entry
 */
static int match_module(zend_module_entry *entry)
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
 *  @param  tsrm_ls
 *  @return Extension*
 */
static ExtensionImpl *find(int number TSRMLS_DC)
{
    // do we already have an extension with this number?
    auto iter = number2extension.find(number);
    if (iter != number2extension.end()) return iter->second;
    
    // no, not yet, loop through all modules
    zend_hash_apply(&module_registry, (apply_func_t)match_module TSRMLS_CC);
    
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
 *  @param  tsrm_ls
 *  @return int         0 on success
 */
int ExtensionImpl::processStartup(int type, int module_number TSRMLS_DC)
{
    // initialize and allocate the "global" variables
    ZEND_INIT_MODULE_GLOBALS(phpcpp, init_globals, NULL); 

    // get the extension
    auto *extension = find(module_number TSRMLS_CC);

    // array contains ini settings
    auto *entries = extension->_ini = new zend_ini_entry[extension->_data->iniVariables()+1];

    // the entry that we're filling
    int i=0;

    // Fill the php.ini entries
    extension->_data->iniVariables([entries, &i, module_number](Ini &ini) {
    
        // initialize the function
        zend_ini_entry *entry = &entries[i];
        
        // fill the property
        ini.fill(entry, module_number);
        
        // move on to the next iteration
        i++;
    });

    // last entry should be set to all zero's
    memset(&entries[i], 0, sizeof(zend_ini_entry));

    // register ini entries in Zend core
    zend_register_ini_entries(entries, module_number TSRMLS_CC);

    // initialize the extension
    extension->initialize(module_number TSRMLS_CC);
    
    // remember that we're initialized (when you use "apache reload" it is 
    // possible that the processStartup() method is called more than once)
    extension->_locked = true;
    
    // is the callback registered?
    if (extension->_onStartup) extension->_onStartup();

    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when the extension is about to be stopped
 *  @param  type        Module type
 *  @param  number      Module number
 *  @param  tsrm_ls
 *  @return int
 */
int ExtensionImpl::processShutdown(int type, int module_number TSRMLS_DC)
{
    // get the extension
    auto *extension = find(module_number TSRMLS_CC);

    // unregister the ini entries
    zend_unregister_ini_entries(module_number TSRMLS_CC);

    // destruct the ini entries
    if (extension->_ini) delete[] extension->_ini;

    // forget the ini entries
    extension->_ini = nullptr;

    // is the callback registered?
    if (extension->_onShutdown) extension->_onShutdown();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when a request starts
 *  @param  type        Module type
 *  @param  number      Module number
 *  @param  tsrm_ls
 *  @return int         0 on success
 */
int ExtensionImpl::processRequest(int type, int module_number TSRMLS_DC)
{
    // get the extension
    auto *extension = find(module_number TSRMLS_CC);
    
    // is the callback registered?
    if (extension->_onRequest) extension->_onRequest();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when a request is ended
 *  @param  type        Module type
 *  @param  number      Module number
 *  @param  tsrm_ls
 *  @return int         0 on success
 */
int ExtensionImpl::processIdle(int type, int module_number TSRMLS_DC)
{
    // get the extension
    auto *extension = find(module_number TSRMLS_CC);
    
    // is the callback registered?
    if (extension->_onIdle) extension->_onIdle();
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Function that is called when the PHP engine initializes with a different PHP-CPP
 *  version for the libphpcpp.so file than the version the extension was compiled for
 *  @param  type        Module type
 *  @param  number      Module number
 *  @param  tsrm_ls
 *  @return int         0 on success
 */
int ExtensionImpl::processMismatch(int type, int module_number TSRMLS_DC)
{
    // get the extension
    auto *extension = find(module_number TSRMLS_CC);
    
    // report a warning
    warning << "Version mismatch between PHP-CPP and extension " << extension->name() << " " << extension->version() << " (recompile needed?)" << std::endl;
    
    // done
    return BOOL2SUCCESS(true);
}

/**
 *  Constructor
 *  @param  data        Pointer to the extension object created by the extension programmer
 *  @param  name        Name of the extension
 *  @param  version     Version number
 *  @param  apiversion  API version number
 */
ExtensionImpl::ExtensionImpl(Extension *data, const char *name, const char *version, int apiversion) : 
    ExtensionBase(data)
{
    // keep extension pointer based on the name
    name2extension[name] = this;
    
    // assign all members (apart from the globals)
    _entry.size = sizeof(zend_module_entry);                       // size of the data
    _entry.zend_api = ZEND_MODULE_API_NO;                          // api number
    _entry.zend_debug = ZEND_DEBUG;                                // debug mode enabled?
    _entry.zts = USING_ZTS;                                        // is thread safety enabled?
    _entry.ini_entry = NULL;                                       // the php.ini record, will be filled by Zend engine
    _entry.deps = NULL;                                            // dependencies on other modules
    _entry.name = name;                                            // extension name
    _entry.functions = NULL;                                       // functions supported by this module (none for now)
    _entry.module_startup_func = &ExtensionImpl::processStartup;   // startup function for the whole extension
    _entry.module_shutdown_func = &ExtensionImpl::processShutdown; // shutdown function for the whole extension
    _entry.request_startup_func = &ExtensionImpl::processRequest;  // startup function per request
    _entry.request_shutdown_func = &ExtensionImpl::processIdle;    // shutdown function per request
    _entry.info_func = NULL;                                       // information for retrieving info
    _entry.version = version;                                      // version string
    _entry.globals_size = 0;                                       // size of the global variables
    _entry.globals_ctor = NULL;                                    // constructor for global variables
    _entry.globals_dtor = NULL;                                    // destructor for global variables
    _entry.post_deactivate_func = NULL;                            // unknown function
    _entry.module_started = 0;                                     // module is not yet started
    _entry.type = 0;                                               // temporary or persistent module, will be filled by Zend engine
    _entry.handle = NULL;                                          // dlopen() handle, will be filled by Zend engine
    _entry.module_number = 0;                                      // module number will be filled in by Zend engine
    _entry.build_id = (char *)ZEND_MODULE_BUILD_ID;                // check if extension and zend engine are compatible

    // things that only need to be initialized
#ifdef ZTS
    _entry.globals_id_ptr = NULL;
#else
    _entry.globals_ptr = NULL;
#endif

    // everything is ok if the api versions match
    if (apiversion == PHPCPP_API_VERSION) return;
    
    // mismatch between api versions, the extension is invalid, we use a 
    // different startup function to report to the user
    _entry.module_startup_func = &ExtensionImpl::processMismatch;

    // the other callback functions are no longer necessary
    _entry.module_shutdown_func = nullptr;
    _entry.request_startup_func = nullptr;
    _entry.request_shutdown_func = nullptr;
}

/**
 *  Destructor
 */
ExtensionImpl::~ExtensionImpl()
{
    // deallocate the php.ini entries
    if (_ini) delete[] _ini;
    
    // deallocate functions
    if (_entry.functions) delete[] _entry.functions;
}

/**
 *  The extension name
 *  @return const char *
 */
const char *ExtensionImpl::name() const
{
    // name is stored in the struct
    return _entry.name;
}

/**
 *  The extension version
 *  @return const char *
 */
const char *ExtensionImpl::version() const
{
    // version is stored in the struct
    return _entry.version;
}

/**
 *  Retrieve the module entry
 *  @return zend_module_entry
 */
zend_module_entry *ExtensionImpl::module()
{
    // check if functions were already defined
    if (_entry.functions) return &_entry;

    // if the 'processMismatch' function is installed, the API version is wrong,
    // and nothing should be initialized
    if (_entry.module_startup_func == &ExtensionImpl::processMismatch) return &_entry;

    // the number of functions
    int count = _data->functions();
    
    // skip if there are no functions
    if (count == 0) return &_entry;

    // allocate memory for the functions
    zend_function_entry *entries = new zend_function_entry[count + 1];

    // index being processed
    int i = 0;

    // apply a function to each function
    _data->functions([&i, entries](const std::string &prefix, NativeFunction &function) {
        
        // initialize the function
        function.initialize(prefix, &entries[i]);
        
        // move on to the next iteration
        i++;
    });

    // last entry should be set to all zeros
    zend_function_entry *last = &entries[count];

    // all should be set to zero
    memset(last, 0, sizeof(zend_function_entry));

    // store functions in entry object
    _entry.functions = entries;

    // return the entry
    return &_entry;
}

/**
 *  Initialize the extension after it was started
 *  @param  module_number
 *  @param  tsrm_ls
 */
void ExtensionImpl::initialize(int module_number TSRMLS_DC)
{
    // the constants are registered after the module is ready
    _data->constants([module_number TSRMLS_CC](const std::string &prefix, Constant &c) {
        
        // forward to implementation class
        c.implementation()->initialize(prefix, module_number TSRMLS_CC);
    });
    
    // we also need to register each class, find out all classes
    _data->classes([TSRMLS_C](const std::string &prefix, ClassBase &c) {
        
        // forward to implementation class
        c.implementation()->initialize(&c, prefix TSRMLS_CC);
    });
}

/**
 *  End of namespace
 */
}

