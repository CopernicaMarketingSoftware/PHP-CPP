/**
 *  Interface.h
 *
 *  @copyright 2014 - 2022 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Interface : private ClassBase
{
public:
    /**
     *  Constructor
     *  @param  name
     */
    Interface(const char *name);

    /**
     *  Destructor
     */
    virtual ~Interface() = default;

    /**
     *  Add a - of course abstract - method to the interface
     *  @param  name        Name of the method
     *  @param  arguments   Optional description of the arguments
     *  @return Interface   Same object to allow chaining
     */
    Interface &method(const char *name, const Arguments &arguments = {})
    {
        // call base
        ClassBase::method(name, Abstract | Public, arguments);

        // return self
        return *this;
    }

    /**
     *  Add a - of course abstract - method to the interface
     *  @param  name        Name of the method
     *  @param  flags       Optional flags
     *  @param  arguments   Optional description of the arguments
     *  @return Interface   Same object to allow chaining
     */
    Interface &method(const char *name, int flags, const Arguments &arguments = {})
    {
        // call base (an interface method is always public, so we add these flags,
        // and although it is always abstract, PHP does not allow this flag, so we
        // remove it in case the extension programmer had set it)
        ClassBase::method(name, (Public | flags) & ~Abstract, arguments);

        // return self
        return *this;
    }
    
    /**
     *  Extends exisiting PHP interface
     *
     *  Note that the interface that you supply must already exist! Therefore
     *  you can only supply interfaces that you created in your own extension.
     *
     *  @param  interface   Interface object
     *  @return Interface   Same object to allow chaining
     */
    Interface &extends(const Interface &interface) { ClassBase::implements(interface); return *this; }

    /**
     *  The namespace needs to have access to the private ClassBase base
     *  class, to actually register the interface.
     */
    friend class Namespace;

    /**
     *  All Php::Class<AnyThing> also need access to the base class to
     *  register an interface.
     */
    template<typename ANYTHING> friend class Class;
};

/**
 *  End namespace
 */
}
