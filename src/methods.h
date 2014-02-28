/**
 *  Methods.h
 *
 *  Class in which all methods are stored. This is a class that is used 
 *  internally by the PHP-CPP library, and that you do not have to use
 *  as an extension writer.
 *
 *  Essentially, it is std::map that is a littlebit extended with features
 *  to initialize the methods in the Zend engine
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Methods
{
public:
    /**
     *  Constructor
     */
    Methods() {}

    /**
     *  Destructor
     */
    virtual ~Methods()
    {
        // destruct the entries
        if (_entries) delete[] _entries;
    }

    /**
     *  Retrieve an array of zend_function_entry objects that hold the 
     *  properties for each method. This method is called at extension
     *  startup time to register all methods.
     * 
     *  @param  classname       The class name
     *  @return zend_function_entry[]
     */
    const struct _zend_function_entry *entries(const std::string &classname)
    {
        // already initialized?
        if (_entries) return _entries;
        
        // allocate memory for the functions
        _entries = new zend_function_entry[_methods.size() + 1];
        
        // keep iterator counter
        int i = 0;

        // loop through the functions
        for (auto &method : _methods)
        {
            // retrieve entry
            zend_function_entry *entry = &_entries[i++];

            // let the function fill the entry
            // @todo check flags for the method
            method->initialize(entry, classname);
        }

        // last entry should be set to all zeros
        zend_function_entry *last = &_entries[i];

        // all should be set to zero
        memset(last, 0, sizeof(zend_function_entry));

        // done
        return _entries;
    }
    
    /**
     *  Add a method
     *  @param  name
     *  @param  method
     */
    void add(MethodBase *method)
    {
        // add the method
        _methods.insert(std::shared_ptr<MethodBase>(method));
    }


private:
    /**
     *  Pointer to the entries
     *  @var    zend_function_entry[]
     */
    struct _zend_function_entry *_entries = nullptr;

    /**
     *  Map of all methods
     *  
     *  A unique_ptr would have been cooler, but that somehow does not compile
     * 
     *  @var    std::set
     */
    std::set<std::shared_ptr<MethodBase>> _methods;

};

/**
 *  End namespace
 */
}

