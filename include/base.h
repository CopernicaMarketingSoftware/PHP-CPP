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
     *  @return Object
     */
    Object &value()
    {
        return _self;
    }

    /**
     *  Convert the object to a Php::Value object (how it is used externally)
     *  @return Object
     */
    const Object &value() const
    {
        return _self;
    }
    
    /**
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const char *name)
    {
        return _self[name];
    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name)
    {
        return _self[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const char *name)
    {
        return _self[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const std::string &name)
    {
        return _self[name];
    }

    /**
     *  Get access to a property by name using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const char *name) const
    {
        return _self[name];
    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name) const
    {
        return _self[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const char *name) const
    {
        return _self[name];
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const std::string &name) const
    {
        return _self[name];
    }
    
protected:
    /**
     *  The zend_object
     *  @var    Value
     */
    Object _self;
   
private:
    /**
     *  Private method to assign the zend object
     *  @param  zend_object
     */
    void assign(Value &&object)
    {
        // copy pointer
        _self = std::move(object);
    }
    
    /**
     *  ClassBase has access to private data
     */
    friend class ClassBase;
};


/**
 *  End of namespace
 */
}

