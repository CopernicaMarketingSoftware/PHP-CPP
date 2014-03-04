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
 *  Forward declarations
 */
class MixedObject;

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
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return HashMember
     */
    HashMember<std::string> operator[](const char *name)
    {
        return Value(this)[name];
    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return HashMember
     */
    HashMember<std::string> operator[](const std::string &name)
    {
        return Value(this)[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return HashMember
     */
    HashMember<std::string> property(const char *name)
    {
        return Value(this)[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return HashMember
     */
    HashMember<std::string> property(const std::string &name)
    {
        return Value(this)[name];
    }

    /**
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const char *name) const
    {
        return Value(this)[name];
    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name) const
    {
        return Value(this)[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const char *name) const
    {
        return Value(this)[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const std::string &name) const
    {
        return Value(this)[name];
    }
    
private:
    /**
     *  Store the object in the zend object cache
     *  @param  entry
     *  @return MixedObject
     */
    MixedObject *store(struct _zend_class_entry *entry);

    /**
     *  Retrieve the handle
     *  @return int
     */
    int handle() const
    {
        return _handle;
    }
    
    /**
     *  The handle in the zend object cache
     *  @var    int
     */
    int _handle = 0;

    /**
     *  Friends that have access to the private members
     */
    friend class Value;
    friend class Object;
    friend class ClassBase;

};


/**
 *  End of namespace
 */
}

