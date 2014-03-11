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

    /**
     *  Overridable method that is called to check if a property is set
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @return bool
     */
    virtual bool __isset(const Php::Value &key);
    
    /**
     *  Overridable method that is called to set a new property
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @param  value
     */
    virtual void __set(const Php::Value &key, const Php::Value &value);
    
    /**
     *  Retrieve a property
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @return value
     */
    virtual Php::Value __get(const Php::Value &key);
    
    /**
     *  Remove a member
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param key
     */
    virtual void __unset(const Php::Value &key);

    /**
     *  Call a method
     * 
     *  This method is called when a method is called from the PHP script that
     *  was not explicitly defined. You can use this to catch variable method
     *  names, or to support all thinkable method names.
     * 
     *  @param  method      Name of the method that was called
     *  @param  params      The parameters that were passed to the function
     *  @return Value       The return value
     */
    virtual Value __call(const char *method, Parameters &params);

    /**
     *  Call the class as if it was a function
     * 
     *  This method is called when a an object is used with () operators:
     *  $object(). You can override this method to make objects callable.
     * 
     *  @param  params      The parameters that were passed to the function
     *  @return Value       The return value
     */
    virtual Value __invoke(Parameters &params);
    
    /**
     *  Cast the object to a string
     * 
     *  This method is called when an object is casted to a string, or when
     *  it is used in a string context
     * 
     *  @return Value       The object as a string
     */
    virtual Value __toString();
    
    /**
     *  Cast the object to an integer
     * 
     *  This method is called when an object is casted to an integer, or when
     *  it is used in an integer context
     * 
     *  @return int         Integer value
     */
    virtual long __toInteger();
    
    /**
     *  Cast the object to a float
     * 
     *  This method is called when an object is casted to a float, or when it
     *  is used in a float context
     * 
     *  @return double      Floating point value
     */
    virtual double __toFloat();
    
    /**
     *  Cast the object to a boolean
     * 
     *  This method is called when an object is casted to a bool, or when it
     *  is used in a boolean context
     * 
     *  @return bool
     */
    virtual bool __toBool();
    
    
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

