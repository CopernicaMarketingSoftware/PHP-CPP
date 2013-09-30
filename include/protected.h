/**
 *  Protected.h
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
class Protected : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Protected(const char *name, const Value &value) : Member(name, false, value) {}
    
    /**
     *  Destructor
     */
    virtual ~Protected() {}
    
};

/**
 *  End of namespace
 */
}

