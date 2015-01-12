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
class ObjectImpl;


/**
 *  Class definition
 */
class Base
{
private:
    /**
     *  Object handle in the PHP engine
     *  @var    ObjectImpl
     */
    ObjectImpl *_impl = nullptr;
    
protected:
    /**
     *  Constructor
     */
    Base() {}

    /**
     *  Copy constructor
     * 
     *  This copy constructor is explicitly defined to make sure that the
     *  copied object does not already have an implementation in the zend engine.
     *  Otherwise the copied object has the same object handle as the original
     *  object.
     * 
     *  @param  base
     */
    Base(const Base &base) : _impl(nullptr) {}
    
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
        return Value(this).get(name);
    }

    /**
     *  Alternative way to access a property using the [] operator
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name) const
    {
        return Value(this).get(name);
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const char *name) const
    {
        return Value(this).get(name);
    }
    
    /**
     *  Retrieve a property by name
     *  @param  string
     *  @return Value
     */
    Value property(const std::string &name) const
    {
        return Value(this).get(name);
    }
    
    /**
     *  Overridable method that is called right before an object is destructed
     */
    void __destruct() const;

    /**
     *  Overridable method that is called right after an object is cloned
     * 
     *  The default implementation does nothing
     */
    void __clone() const {}
    
    /**
     *  Overridable method that is called to check if a property is set
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @return bool
     */
    bool __isset(const Php::Value &key) const;
    
    /**
     *  Overridable method that is called to set a new property
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @param  value
     */
    void __set(const Php::Value &key, const Php::Value &value) const;
    
    /**
     *  Retrieve a property
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param  key
     *  @return value
     */
    Php::Value __get(const Php::Value &key) const;
    
    /**
     *  Remove a member
     * 
     *  The default implementation does nothing, and the script will fall back
     *  to accessing the regular object properties
     * 
     *  @param key
     */
    void __unset(const Php::Value &key) const;

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
    Php::Value __call(const char *method, Php::Parameters &params) const;

    /**
     *  Call the class as if it was a function
     * 
     *  This method is called when a an object is used with () operators:
     *  $object(). You can override this method to make objects callable.
     * 
     *  @param  params      The parameters that were passed to the function
     *  @return Value       The return value
     */
    Php::Value __invoke(Php::Parameters &params) const;
    
    /**
     *  Cast the object to a string
     * 
     *  This method is called when an object is casted to a string, or when
     *  it is used in a string context
     * 
     *  @return Value       The object as a string
     */
    Php::Value __toString() const;
    
    /**
     *  Cast the object to an integer
     * 
     *  This method is called when an object is casted to an integer, or when
     *  it is used in an integer context
     * 
     *  @return int         Integer value
     */
    Php::Value __toInteger() const;
    
    /**
     *  Cast the object to a float
     * 
     *  This method is called when an object is casted to a float, or when it
     *  is used in a float context
     * 
     *  @return double      Floating point value
     */
    Php::Value __toFloat() const;
    
    /**
     *  Cast the object to a boolean
     * 
     *  This method is called when an object is casted to a bool, or when it
     *  is used in a boolean context
     * 
     *  @return bool
     */
    Value __toBool() const;

    /**
     *  Compare the object with a different object
     *  
     *  Check how a different object compares to this object
     * 
     *  @param  that        Object to compare with
     *  @return int
     */
    int __compare(const Base &base) const;
    

private:
    /**
     *  Get access to the implementation object
     *  @return ObjectImpl
     */
    const ObjectImpl *implementation() const
    {
        return _impl;
    }

    /**
     *  Classes that have direct access to private date
     */
    friend class ObjectImpl;
    friend class Object;
    friend class Value;
};

/**
 *  End of namespace
 */
}

