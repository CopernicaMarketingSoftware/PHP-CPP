/**
 *  Interface.h
 *
 *  @copyright 2014 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Interface : private ClassBase
{
public:
    /**
     *  Constructor
     *  @param  name
     */
    Interface(const char *name) : ClassBase(name, ClassType::Interface) {}
    
    /**
     *  Destructor
     */
    virtual ~Interface() {}
    
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

