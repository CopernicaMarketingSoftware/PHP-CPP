/**
 *  FinalClass.h
 *
 *  An final class can not be extended
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
class FinalClass : public Class<T>
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the class
     */
    FinalClass(const char *name) : Class<T>(name, 0x40) {}
    
    /**
     *  Destructor
     */
    virtual ~FinalClass() {}
};

/**
 *  End namespace
 */
}

