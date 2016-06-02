/**
 *  Namespace.h
 *
 *  Class that can be used to group various functions and classes into one
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
class NativeFunction;

/**
 *  Class definition
 */
class PHPCPP_EXPORT Namespace
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
    std::list<std::shared_ptr<NativeFunction>> _functions;

    /**
     *  Classes defined in the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<ClassBase>> _classes;

    /**
     *  Constants defined in the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<Constant>> _constants;

    /**
     *  Namespaces defined inside the namespace
     *  @var    list
     */
    std::list<std::shared_ptr<Namespace>> _namespaces;

    /**
     *  Add a native function directly to the namespace
     *
     *  @param  name        Name of the function
     *  @param  function    The raw function to add
     *  @param  arguments   Optional argument specification
     *  @return Same object to allow chaining
     */
    Namespace &add(const char *name, ZendCallback function, const Arguments &arguments = {});

    /**
     *  Is the object locked?
     *
     *  After the object is locked, no more elements can be added to it.
     *  This happens after the call to get_module - it no longer makes
     *  sense to add more objects. When 'apache reload' is executed, the
     *  get_module() function is called for a second (or third, or fourth)
     *  time, but the classes, functions and namespaces will then not be
     *  filled.
     *
     *  @return bool
     */
    virtual bool locked() const
    {
        // by default, namespaces are not locked (only derived extension
        // objects can end up in a locked state
        return false;
    }


public:
    /**
     *  Constructor
     *  @param  name        Name of the namespace
     */
    Namespace(const char *name) : _name(name) {}

    /**
     *  Destructor
     */
    virtual ~Namespace() = default;

    /**
     *  Add a native function directly to the namespace
     *
     *  @param  name        Name of the function
     *  @param  arguments   Optional argument specification
     *  @return Same object to allow chaining
     */
    template <void  (*callback)()>                          Namespace &add(const char *name, const Arguments &arguments = {}) { return add(name, &ZendCallable::invoke<callback>, arguments); }
    template <void  (*callback)(Parameters &parameters)>    Namespace &add(const char *name, const Arguments &arguments = {}) { return add(name, &ZendCallable::invoke<callback>, arguments); }
    template <Value (*callback)()>                          Namespace &add(const char *name, const Arguments &arguments = {}) { return add(name, &ZendCallable::invoke<callback>, arguments); }
    template <Value (*callback)(Parameters &parameters)>    Namespace &add(const char *name, const Arguments &arguments = {}) { return add(name, &ZendCallable::invoke<callback>, arguments); }

    /**
     *  Add a native function directly to the namespace
     *  @param  name        Name of the function
     *  @param  function    The function to add
     *  @param  arguments   Optional argument specification
     *  @return Namespace   Same object to allow chaining
     */
    PHPCPP_DEPRECATED Namespace &add(const char *name, const native_callback_0 &function, const Arguments &arguments = {});
    PHPCPP_DEPRECATED Namespace &add(const char *name, const native_callback_1 &function, const Arguments &arguments = {});
    PHPCPP_DEPRECATED Namespace &add(const char *name, const native_callback_2 &function, const Arguments &arguments = {});
    PHPCPP_DEPRECATED Namespace &add(const char *name, const native_callback_3 &function, const Arguments &arguments = {});

    /**
     *  Add a native class to the namespace by moving it
     *  @param  type        The class implementation
     *  @return Namespace   Same object to allow chaining
     */
    template<typename T>
    Namespace &add(Class<T> &&type)
    {
        // skip when locked
        if (locked()) return *this;

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
        // skip when locked
        if (locked()) return *this;

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
        // skip when locked
        if (locked()) return *this;

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
        // skip when locked
        if (locked()) return *this;

        // make a copy and add it to the list of classes
        _classes.push_back(std::unique_ptr<ClassBase>(new Interface(interface)));

        // allow chaining
        return *this;
    }

    /**
     *  Add a constant to the namespace
     *  @param  constant    The constant to add
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(Constant &&constant)
    {
        // skip when locked
        if (locked()) return *this;

        // and add it to the list of constants
        _constants.push_back(std::unique_ptr<Constant>(new Constant(std::move(constant))));

        // allow chaining
        return *this;
    }

    /**
     *  Add a constant to the namespace
     *  @param  constant    The constant to add
     *  @return Namespace   Same object to allow chaining
     */
    Namespace &add(const Constant &constant)
    {
        // skip when locked
        if (locked()) return *this;

        // and add it to the list of constants
        _constants.push_back(std::unique_ptr<Constant>(new Constant(constant)));

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
        // skip when locked
        if (locked()) return *this;

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
        // skip when locked
        if (locked()) return *this;

        // make a copy and add it to the list of namespaces
        _namespaces.push_back(std::unique_ptr<Namespace>(new Namespace(ns)));

        // allow chaining
        return *this;
    }

    /**
     *  The total number of functions
     *  @return size_t
     */
    size_t functions() const
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
    void functions(const std::function<void(const std::string &ns, NativeFunction &func)> &callback);

    /**
     *  Apply a callback to each registered class
     *
     *  The callback will be called with the name of the namespace, and
     *  a reference to the registered class.
     *
     *  @param  callback
     */
    void classes(const std::function<void(const std::string &ns, ClassBase &clss)> &callback);

    /**
     *  Apply a callback to each registered constant
     *
     *  The callback will be called with the name of the namespace, and
     *  a reference to the registered constant
     *
     *  @param  callback
     */
    void constants(const std::function<void(const std::string &ns, Constant &constant)> &callback);

};

/**
 *  End namespace
 */
}
