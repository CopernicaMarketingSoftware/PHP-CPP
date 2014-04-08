/**
 *  Namespace.h
 * 
 *  Class that can be used to group  various functions and classes into one
 *  namespace.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declaration
 */
class Function;

/**
 *  Class definition
 */
class Namespace
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the namespace
     */
    Namespace(const char *name) : _name(name) {}
    
    /**
     *  Copy constructor
     *  @param  ns          Namespace to copy
     */
    Namespace(const Namespace &ns) : 
        _name(ns._name), 
        _functions(ns._functions),
        _classes(ns._classes),
        _namespaces(ns._namespaces) {}
        
    /**
     *  Move constructor
     *  @param  ns
     */
    Namespace(Namespace &&ns) :
        _name(std::move(ns._name)), 
        _functions(std::move(ns._functions)),
        _classes(std::move(ns._classes)),
        _namespaces(std::move(ns._namespaces)) {}
    
    /**
     *  Destructor
     */
    virtual ~Namespace() {}
    
    /**
     *  Add a native function directly to the extension
     *  @param  name        Name of the function
     *  @param  function    The function to add
     *  @param  arguments   Optional argument specification
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(const char *name, const native_callback_0 &function, const Arguments &arguments = {});
    Namespace &add(const char *name, const native_callback_1 &function, const Arguments &arguments = {});
    Namespace &add(const char *name, const native_callback_2 &function, const Arguments &arguments = {});
    Namespace &add(const char *name, const native_callback_3 &function, const Arguments &arguments = {});
    
    /**
     *  Add a native class to the extension by moving it
     *  @param  type        The class implementation
     *  @return Namespace   Same object to allow chaining
     */
    template<typename T>
    Namespace &add(Class<T> &&type)
    {
        // make a copy of the object
        auto *copy = new Class<T>(std::move(type));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a native class to the extension by copying it
     *  @param  type        The class implementation
     *  @param  Namespace   Same object to allow chaining
     */
    template<typename T>
    Namespace &add(const Class<T> &type)
    {
        // make a copy of the object
        auto *copy = new Class<T>(std::move(type));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add an interface to the extension by moving it
     *  @param  interface   The interface properties
     */
    Namespace &add(Interface &&interface)
    {
        // make a copy of the object
        auto *copy = new Interface(std::move(interface));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add an interface to the extension by copying it
     *  @param  interface   The interface properties
     */
    Namespace &add(const Interface &interface)
    {
        // make a copy of the object
        auto *copy = new Interface(interface);
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
        
        // allow chaining
        return *this;
    }
    
    /**
     *  Add a namespace to the extension by moving it
     *  @param  ns          The namespace
     */
    Namespace &add(Namespace &&ns)
    {
        // make a copy of the object
        auto *copy = new Namespace(std::move(ns));
        
        // add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(copy));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a namespace to the extension by copying it
     *  @param  ns          The namespace
     */
    Namespace &add(const Namespace &ns)
    {
        // make a copy of the object
        auto *copy = new Namespace(std::move(ns));
        
        // add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(copy));
        
        // allow chaining
        return *this;
    }

    
    /**
     *  Add a ini entry to the extension by moving it
     *  @param  ini        The class implementation
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(Ini &&ini)
    {
        // make a copy of the object
        auto *copy = new Ini(std::move(ini));
        
        // and add it to the list of classes
        _ini_entries.push_back(std::unique_ptr<Ini>(copy));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a ini entry to the extension by copying it
     *  @param  ini        The class implementation
     *  @param  Namespace   Same object to allow chaining
     */
    Namespace &add(const Ini &ini)
    {
        // make a copy of the object
        auto *copy = new Ini(std::move(ini));
        
        // and add it to the list of classes
        _ini_entries.push_back(std::unique_ptr<Ini>(copy));
        
        // allow chaining
        return *this;
    }


protected:
    /**
     *  Name of the namespace
     *  @var    string
     */
    std::string _name;

    /**
     *  Functions defined by the extension
     *  @var    list
     */
    std::list<std::shared_ptr<Function>> _functions;

    /**
     *  Classes defined by the extension
     *  @var    list
     */
    std::list<std::shared_ptr<ClassBase>> _classes;
    
    /**
     *  Namespaces defined by the extension
     *  @var    list
     */
    std::list<std::shared_ptr<Namespace>> _namespaces;

    /**
     *  Ini entry defined by the extension
     *  @var    list
     */
    std::list<std::shared_ptr<Ini>> _ini_entries;
    
    /**
     *  The total number of functions
     *  @return size_t
     */
    size_t functions()
    {
        // number of functions in this namespace
        int result = _functions.size();
        
        // number of functions in sub-namespace
        for (auto &ns : _namespaces) result += ns->functions();
        
        // done
        return result;
    }

    /**
     *  Initialize all functions in this namespace
     *  @param  ns          Namespace prefix
     *  @param  entries     The array to be filled
     *  @return int         Number of functions that were initialized
     */
    size_t initialize(const std::string &ns, struct _zend_function_entry entries[]);

    /**
     *  Initialize the namespace after it was registered
     *  @param  parent      Parent namespace
     *  @param  tsrm_ls
     */
    void initialize(const std::string &parent TSRMLS_DC);
};
    
/**
 *  End namespace
 */
}

