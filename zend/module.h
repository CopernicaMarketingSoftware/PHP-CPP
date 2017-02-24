/**
 *  Module.h
 *
 *  In PHP scripts you can use the dl() function to open additional extensions.
 *  This builtin dl() function call is protected with all sorts of security measures,
 *  like checking whether a filename contains slashes or otherwise potentially
 *  dangerous things. This is all understandable, because you do not want a kid
 *  from an ~user directory to be able to make changes to the globalle running
 *  apache process, and for example make changes to catch data from other websites
 *  running on the same host.
 * 
 *  However, people who are in a position to write C++ and deploy code however, 
 *  already _ARE_ in a position to do dangerous things. For them we do not want
 *  these safety checks. In fact, we want to offer raw access, so that they can
 *  open modules - no matter what. So we had to make our own copy of the dl()
 *  function, and because we're here in C++ context, we do it a little nicer
 *  than the original C++ code. This module class is a utility class used by
 *  our own dl() implementation.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {
    
/**
 *  Class definition
 */
class Module
{
private:
    /**
     *  The handle of the module
     *  @var void*
     */
    void *_handle;
    
    /**
     *  The module entry
     *  @var zend_module_entry
     */
    zend_module_entry *_entry = nullptr;
    

    /**
     *  Internal helper class with persistent modules
     */
    class Persistent
    {
    private:
        /**
         *  The set of handles
         *  @var std::set
         */
        std::set<void*> _handles;
        
    public:
        /**
         *  Constructor
         */
        Persistent() {}
        
        /**
         *  Destructor
         */
        virtual ~Persistent() 
        {
            // remove all handles
            while (!_handles.empty())
            {
                // get first handle
                auto iter = _handles.begin();
                
                // remove the handle
                DL_UNLOAD(*iter);
                
                // remove from set
                _handles.erase(iter);
            }
        }
        
        /**
         *  Check whether a handle is already persistently opened
         *  @param  handle
         *  @return bool
         */
        bool contains(void *handle) const
        {
            return _handles.find(handle) != _handles.end();
        }
        
        /**
         *  Add a library persistently
         *  @param  module
         */
        void add(const char *module)
        {
            // insert the handle
            _handles.insert(DL_LOAD(module));
        }
    };

    /**
     *  All persistent modules
     *  @var Persistent
     */
    static Persistent _persistent;

public:
    /**
     *  Constructor
     * 
     *  A module can be loaded persistently. This means that the variables in
     *  the module will keep in scope for as long as Apache runs, even though
     *  the extension is not active in other page views
     * 
     *  @param  module          Name of the module
     *  @param  persistent      Should it be loaded persistently
     */
    Module(const char *module, bool persistent)
    {
        // the path we're going to load
        ExtensionPath path(module);
        
        // load the module
        _handle = DL_LOAD(path);
        
        // handle should be valid
        if (!_handle) return;
        
        // if we have to open it persistently, we open it for a second time so that
        // the refcounter always stays 1 or higher
        if (persistent && !_persistent.contains(_handle)) _persistent.add(module);
        
        // we have to call the get_module() function
        Symbol<zend_module_entry*()> get_module(_handle, "get_module");
        
        // was the get_module() function found
        if (!get_module) return;

        // retrieve the module entry
        _entry = get_module();
    }
    
    /**
     *  Destructor
     */
    virtual ~Module()
    {
        // if the handle is still valid, we have to unload it
      if (_handle) DL_UNLOAD((DL_HANDLE)_handle);
    }
    
    /**
     *  Check if the module is valid
     *  @return bool
     */
    bool valid() const
    {
        // module-entry must exist
        if (!_handle || !_entry) return false;
    
        // check api compatibility
        if (_entry->zend_api != ZEND_MODULE_API_NO) return false;
        
        // and other stuff
        return strcmp(_entry->build_id, ZEND_MODULE_BUILD_ID) == 0;
    }
    
    /**
     *  Start the module
     *  @return bool
     */
    bool start()
    {
        // this is not possible if the module is invalid in the first place
        if (!valid()) return false;
        
        // the Zend engine sets a number of properties in the entry class, we do that here too
        // note that it would be better to call zend_next_free_module() to find the next module
        // number, but some users complain that this function is not always available
        _entry->type = MODULE_TEMPORARY;
        _entry->module_number = zend_hash_num_elements(&module_registry) + 1;
        _entry->handle = _handle;
        
        // @todo does loading an extension even work in a multi-threading setup?
        
        // register the module, this apparently returns a copied entry pointer
        auto *entry = zend_register_module_ex(_entry);

        // forget the entry, so that a new call to start() will fail too
        _entry = nullptr;
        
        // leap out on failure
        if (entry == NULL) return false;

        // startup the module
        if (zend_startup_module_ex(entry) == FAILURE) return false;

        // was a startup-function defined? if not
        if (entry->request_startup_func)
        {
            // call the startup function
            if (entry->request_startup_func(MODULE_TEMPORARY, entry->module_number) == FAILURE) return false;
        }
        
        // all is ok, we can forget about the handle now, so that is won't get destructed
        _handle = nullptr;

        // enable full-table-cleanup, because inside the Zend-engine this is done too
        EG(full_tables_cleanup) = 1;
        
        // we're ready
        return true;
    }
};
    
/**
 *  End of namespace
 */
}

