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
     *  Move constructor is passed to the parent
     *  @param  value
     */
    Object(Value &&value) : Value(std::move(value)) 
    {
        // throw exception in case of problems
        if (value.type() != Type::Object) throw FatalError("Constructing an object variable by moving a non object");
    }

    /**
     *  Copy constructor is valid if the passed in object is also an object,
     *  or when it is a string holding a classname
     *  @param  that        An other object
     */
    Object(const Value &value) : Value()
    {
        // when a string is passed in, we are going to make a new instance of the
        // passed in string
        if (value.isString()) 
        {
            // instantiate the object
            instantiate(value);
            
            // and call the __construct method
            call("__construct");
        }
        else 
        {
            // this simply copies the other object
            operator=(value);
        }
    }

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
     *  @param  arg0        Optional argument 1
     *  @param  arg1        Optional argument 2
     *  @param  arg2        Optional argument 3
     *  @param  arg3        Optional argument 4
     *  @param  arg4        Optional argument 5
     *  @param  arg5        Optional argument 6
     *  @param  arg6        Optional argument 7
     *  @param  arg7        Optional argument 8
     *  @param  arg8        Optional argument 9
     *  @param  arg9        Optional argument 10
     */
    Object(const char *name) { instantiate(name); call("__construct"); }
    Object(const char *name, Value p0) { instantiate(name); call("__construct", p0); }
    Object(const char *name, Value p0, Value p1) { instantiate(name); call("__construct", p0, p1); }
    Object(const char *name, Value p0, Value p1, Value p2) { instantiate(name); call("__construct", p0, p1, p2); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3) { instantiate(name); call("__construct", p0, p1, p2, p3); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4) { instantiate(name); call("__construct", p0, p1, p2, p3, p4); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5) { instantiate(name); call("__construct", p0, p1, p2, p3, p4, p5); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6) { instantiate(name); call("__construct", p0, p1, p2, p3, p4, p5, p6); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7) { instantiate(name); call("__construct", p0, p1, p2, p3, p4, p5, p6, p7); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8) { instantiate(name); call("__construct", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
    Object(const char *name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8, Value p9) { instantiate(name); call("__construct", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
    
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
     *  @param  name        Class name
     */
    void instantiate(const char *name);
    
};

/**
 *  End namespace
 */
}
