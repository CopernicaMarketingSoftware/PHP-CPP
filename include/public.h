/**
 *  Public.h
 *
 *  Class for adding public properties to a class
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
class Public : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Public(const char *name, const Value &value) : Member(name, true, value) {}
    
    /**
     *  Destructor
     */
    virtual ~Public() {}
    
    
};

/**
 *  End of namespace
 */
}

