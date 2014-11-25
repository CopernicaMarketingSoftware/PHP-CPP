/**
 *  Object.h
 *
 *  Extended Value that can be used to instantiate new objects, and to turn
 *  Php::Base objects into regular Php::Value instances
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {
    
/**
 *  Class definition
 */
class Object : public Value
{
public:
    /**
     *  Constructor for an empty stdClass object
     */
    Object() : Value(Type::Object) {}

    /**
     *  Copy constructor is valid if the passed in object is also an object,
     *  or when it is a string holding a classname
     *  @param  that        An other object
     */
    Object(const Value &value);

    /**
     *  Constructor to create a new instance of a builtin class
     *  
     *  You can use this constructor if you have created an instance of your
     *  own class, but has not assigned it to a variable yet. This happens
     *  for example for classes that are not constructed from PHP userspace,
     *  but from your own functions:
     * 
     *  Php::Value yourFunction()
     *  {
     *      return Php::Object("MyClass", new MyClass());
     *  }
     * 
     *  When you construct objects like this, the __construct function is not
     *  going to be called. If you want to construct the object just as if it
     *  was constructed from PHP user space, do this:
     * 
     *  Php::Value yourFunction()
     *  {
     *      return Php::Object("MyClass");
     *  }
     * 
     *  @param  name        Name of the class to instantiate
     *  @param  base        Implementation of the class
     */
    Object(const char *name, Base *base);

    /**
     *  Wrap around an object implemented by us
     *  @param  object      Object to be wrapped
     */
    Object(Base *base) : Value(base) {}

    /**
     *  Constructor to create a new instance
     * 
     *  This constructor comes in many different forms, to support all possible
     *  number of parameters that are passed to the constructor
     *  
     *  @param  name        Name of the class to instantiate
     *  @param  args        Optional arguments
     */
    template <typename ...Args>
    Object(const char *name, Args&&... args) : Value() { if (instantiate(name)) call("__construct", std::forward<Value>(args)...); }
    
    /**
     *  Destructor
     */
    virtual ~Object() {}

    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    virtual Value &setType(Type type) override
    {
        // throw exception if things are going wrong
        if (type != Type::Object) throw FatalError("Changing type of a fixed object variable");
        
        // call base
        return Value::setType(type);
    }

    /**
     *  Assignment operator
     *  @param  value
     *  @return ForcedValue
     */
    Object &operator=(const Value &value)
    {
        // skip self assignment
        if (this == &value) return *this;
        
        // type must be valid
        if (value.type() != Type::Object) throw FatalError("Assigning a non-object to an object variable");
        
        // call base
        Value::operator=(value);

        // done
        return *this;
    }
    
    /**
     *  Move assignment operator
     *  @param  value
     *  @return ForcedValue
     */
    Object &operator=(Value &&value)
    {
        // skip self assignment
        if (this == &value) return *this;
        
        // type must be valid
        if (value.type() != Type::Object) throw FatalError("Moving a non-object to an object variable");
        
        // call base
        Value::operator=(std::move(value));

        // done
        return *this;
    }

private:
    /**
     *  Helper method to instantiate an object
     * 
     *  This method returns true if there is a __construct() function, and
     *  false otherwise
     * 
     *  @param  name        Class name
     *  @return bool
     */
    bool instantiate(const char *name);
};

/**
 *  End namespace
 */
}
