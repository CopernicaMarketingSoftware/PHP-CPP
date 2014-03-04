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
class Object : public ForcedValue<Type::Object>
{
public:
    /**
     *  Constructor for an empty stdClass object
     */
    Object() : ForcedValue<Type::Object>() {}

    /**
     *  Copy and move constructors are passed on to the base class
     *  @param  that        An other object
     */
    Object(const Value &value) : ForcedValue<Type::Object>(value) {}
    Object(Value &&value) : ForcedValue<Type::Object>(std::move(value)) {}

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
