/**
 *  Base class for defining your own objects
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Base
{
public:
    /**
     *  Constructor
     */
    Base() {}

    /**
     *  Virtual destructor
     */
    virtual ~Base() {}

    /**
     *  The pseudo constructor that is called from PHP after the object is constructed
     *  @param  environment
     *  @param  parameters
     */
    virtual void __construct(Environment &environment, const Parameters &parameters)
    {
        // call all other possible implementations
        __construct(environment);
        __construct(parameters);
        __construct();
    }
    
    /**
     *  The pseudo constructor that is called from PHP after the object is constructed
     *  @param  environment
     */
    virtual void __construct(Environment &environment) {}

    /**
     *  The pseudo constructor that is called from PHP after the object is constructed
     *  @param  parameters
     */
    virtual void __construct(const Parameters &parameters) {}

    /**
     *  The pseudo constructor that is called from PHP after the object is constructed
     */
    virtual void __construct() {}

    /**
     *  The pseudo destructor that is called from PHP right before the object is destructed
     *  @param  environment
     */
    virtual void __destruct(Environment &environment)
    {
        // call the other implementation
        __destruct();
    }
    
    /**
     *  The pseudo destructor that is called from PHP right before the object is destructed
     */
    virtual void __destruct() {}

    /**
     *  Get access to a property by name
     *  @param  string
     *  @return Property
     */
//    Property operator[](const char *name);
    
    /**
     *  Alternative way to access a property
     *  @param  string
     *  @return Property
     */
//    Property operator[](const std::string &name);

protected:
    /**
     *  All properties of the object
     *  @var    Properties
     */
//    Properties _properties;

private:
};

/**
 *  Definition of a method
 */
typedef void    (Base::*method_callback_0)();
typedef void    (Base::*method_callback_1)(Parameters &);
typedef void    (Base::*method_callback_2)(Environment &);
typedef void    (Base::*method_callback_3)(Environment &, Parameters &);
typedef Value   (Base::*method_callback_4)();
typedef Value   (Base::*method_callback_5)(Parameters &);
typedef Value   (Base::*method_callback_6)(Environment &);
typedef Value   (Base::*method_callback_7)(Environment &, Parameters &);

/**
 *  End of namespace
 */
}

