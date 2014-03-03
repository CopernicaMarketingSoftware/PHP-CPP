/**
 *  Object.h
 *
 *  Extended Value that can be used to instantiate new objects, and to turn
 *  Php::Base objects into regular Php::Value instances
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
class Object : public ForcedValue<Type::Object>
{
public:
    /**
     *  Constructor for an empty stdClass object
     * 
     *  @todo   check if this indeed leads to a stdClass instance
     */
    Object() : ForcedValue<Type::Object>() {}

    /**
     *  @todo   copy constructor, move constructor
     */

    /**
     *  Constructor to create a new instance
     * 
     *  This constructor comes in many different forms, to support all possible
     *  number of parameters that are passed to the constructor
     *  
     *  @param  name        Name of the class to instantiate
     *  @param  arg0        Optional argument 1
     *  @param  arg1        Optional argument 2
     *  @param  arg2        Optional argument 3
     *  @param  arg3        Optional argument 4
     *  @param  arg4        Optional argument 5
     *  @param  arg5        Optional argument 6
     *  @param  arg6        Optional argument 7
     *  @param  arg7        Optional argument 8
     *  @param  arg8        Optional argument 9
     *  @param  arg9        Optional argument 10
     */
    Object(const char *name);
    
    /**
     *  Destructor
     */
    virtual ~Object() {}
    
};

/**
 *  End namespace
 */
}
