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

    // @todo    should we delete the copy and move operators because we do not
    //          allow the CPP code to make copies of itself?


    /**
     *  Virtual destructor
     */
    virtual ~Base() {}

    /**
     *  Convert the object to a Php::Value object (how it is used externally)
     *  @return Object
     */
//    Object value();

    /**
     *  Convert the object to a Php::Value object (how it is used externally)
     *  @return Object
     */
//    Object value() const;
    
    /**
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return HashMember
     */
//    HashMember<std::string> operator[](const char *name)
//    {
//        return value()[name];
//    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return HashMember
     */
//    HashMember<std::string> operator[](const std::string &name)
//    {
//        return value()[name];
//    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return HashMember
     */
//    HashMember<std::string> property(const char *name)
//    {
//        return value()[name];
//    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return HashMember
     */
//    HashMember<std::string> property(const std::string &name)
//    {
//        return value()[name];
//    }

    /**
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return Value
     */
//    Value operator[](const char *name) const
//    {
//        return value()[name];
//    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return Value
     */
//    Value operator[](const std::string &name) const
//    {
//        return value()[name];
//    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
//    Value property(const char *name) const
//    {
//        return value()[name];
//    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
//    Value property(const std::string &name) const
//    {
//        return value()[name];
//    }
    
private:
};


/**
 *  End of namespace
 */
}

