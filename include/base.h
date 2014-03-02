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
protected:
    /**
     *  Constructor
     */
    Base() {}

public:
    /**
     *  Virtual destructor
     */
    virtual ~Base() {}

    /**
     *  Convert the object to a Php::Value object (how it is used externally)
     *  @return Value
     */
    Value value() const;
    
    /**
     *  Get access to a property by name
     *  @param  string
     *  @return Value
     */
    Value operator[](const char *name) const
    {
        return value()[name];
    }

    /**
     *  Alternative way to access a property
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name) const
    {
        return value()[name];
    }
};


/**
 *  End of namespace
 */
}

