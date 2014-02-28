/**
 *  AbstractClass.h
 *
 *  An abstract class can not be instantiated, it can only be extended
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
template <typename T>
class AbstractClass : public Class<T>
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the class
     */
    AbstractClass(const char *name) : Class<T>(name, 0x20) {}
    
    /**
     *  Destructor
     */
    virtual ~AbstractClass() {}
};

/**
 *  End namespace
 */
}

