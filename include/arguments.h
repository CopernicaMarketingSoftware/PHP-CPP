/**
 *  Arguments.h
 *
 *  When a function is invoked, it is passed a vector of arguments. This
 *  arguments class, that overrides from vector, takes care of that.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Class definition
 */
class Arguments : public std::vector<Value>
{
public:
    /**
     *  Constructor
     *  @param  argc    The number of arguments
     */
    Arguments(int argc);
    
    /**
     *  Destructor
     */
    virtual ~Arguments() {}

};

/**
 *  End of namespace
 */
}

