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
     */
    void add(const char *name, const native_callback_0 &function, const Arguments &arguments = {});
    void add(const char *name, const native_callback_1 &function, const Arguments &arguments = {});
    void add(const char *name, const native_callback_2 &function, const Arguments &arguments = {});
    void add(const char *name, const native_callback_3 &function, const Arguments &arguments = {});
    
    /**
     *  Add a native class to the extension by moving it
     *  @param  type        The class implementation
     */
    template<typename T>
    void add(Class<T> &&type)
    {
        // make a copy of the object
        auto *copy = new Class<T>(std::move(type));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
    }

    /**
     *  Add a native class to the extension by copying it
     *  @param  type        The class implementation
     */
    template<typename T>
    void add(const Class<T> &type)
    {
        // make a copy of the object
        auto *copy = new Class<T>(std::move(type));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
    }

    /**
     *  Add an interface to the extension by moving it
     *  @param  interface   The interface properties
     */
    void add(Interface &&interface)
    {
        // make a copy of the object
        auto *copy = new Interface(std::move(interface));
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
    }

    /**
     *  Add an interface to the extension by copying it
     *  @param  interface   The interface properties
     */
    void add(const Interface &interface)
    {
        // make a copy of the object
        auto *copy = new Interface(interface);
        
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(copy));
    }
    
    /**
     *  Add a namespace to the extension by moving it
     *  @param  ns          The namespace
     */
    void add(Namespace &&ns)
    {
        // make a copy of the object
        auto *copy = new Namespace(std::move(ns));
        
        // add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(copy));
    }

    /**
     *  Add a namespace to the extension by copying it
     *  @param  ns          The namespace
     */
    void add(const Namespace &ns)
    {
        // make a copy of the object
        auto *copy = new Namespace(std::move(ns));
        
        // add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(copy));
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
     */
    void initialize(const std::string &parent)
    {
        // the namespace to use
        std::string prefix = parent.size() ? parent + "\\" + _name : _name;
        
        // loop through the classes in this namespace
        for (auto &c : _classes) c->initialize(prefix);
        
        // and loop through the other namespaces
        for (auto &n : _namespaces) n->initialize(prefix);
    }
};
    
/**
 *  End namespace
 */
}

