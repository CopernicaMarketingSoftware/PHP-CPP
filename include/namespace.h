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
protected:
    /**
     *  Name of the namespace
     *  @var    string
     */
    std::string _name;

    /**
     *  Functions defined in the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<Function>> _functions;

    /**
     *  Classes defined in the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<ClassBase>> _classes;
    
    /**
     *  Namespaces defined inside the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<Namespace>> _namespaces;

public:
    /**
     *  Constructor
     *  @param  name        Name of the namespace
     */
    Namespace(const char *name) : _name(name) {}
    
    /**
     *  Destructor
     */
    virtual ~Namespace() {}
    
    /**
     *  Add a native function directly to the namespace
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
     *  Add a native class to the namespace by moving it
     *  @param  type        The class implementation
     *  @return Namespace   Same object to allow chaining
     */
    template<typename T>
    Namespace &add(Class<T> &&type)
    {
        // make a copy of the object, and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(new Class<T>(std::move(type))));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a native class to the namespace by copying it
     *  @param  type        The class implementation
     *  @return Namespace   Same object to allow chaining
     */
    template<typename T>
    Namespace &add(const Class<T> &type)
    {
        // and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(new Class<T>(type)));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add an interface to the namespace by moving it
     *  @param  interface   The interface properties
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(Interface &&interface)
    {
        // make a copy and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(new Interface(std::move(interface))));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add an interface to the namespace by copying it
     *  @param  interface   The interface properties
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(const Interface &interface)
    {
        // make a copy and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(new Interface(interface)));
        
        // allow chaining
        return *this;
    }
    
    /**
     *  Add a namespace to the namespace by moving it
     *  @param  ns          The namespace
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(Namespace &&ns)
    {
        // add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(new Namespace(std::move(ns))));
        
        // allow chaining
        return *this;
    }

    /**
     *  Add a namespace to the namespace by copying it
     *  @param  ns          The namespace
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(const Namespace &ns)
    {
        // make a copy and add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(new Namespace(ns)));
        
        // allow chaining
        return *this;
    }

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
     *  Apply a callback to each registered function
     * 
     *  The callback will be called with the name of the namespace, and
     *  a reference to the registered function.
     * 
     *  @param  callback
     */
    void functions(const std::function<void(const std::string &ns, Function &func)> &callback);
    
    /**
     *  Apply a callback to each registered class
     * 
     *  The callback will be called with the name of the namespace, and
     *  a reference to the registered class.
     * 
     *  @param  callback
     */
    void classes(const std::function<void(const std::string &ns, ClassBase &clss)> &callback);
    
};
    
/**
 *  End namespace
 */
}

