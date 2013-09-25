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


};

/**
 *  End of namespace
 */
}

