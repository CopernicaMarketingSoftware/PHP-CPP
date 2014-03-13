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
     */
    void method(const char *name, const Arguments &arguments = {})
    {
        // call base
        ClassBase::method(name, Abstract | Public, arguments);
    }

private:
    /**
     *  Namespaces have access to the private base class
     */
    friend class Namespace;
};

/**
 *  End namespace
 */
}

