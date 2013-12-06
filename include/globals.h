/**
 *  Globals.h
 *
 * 	Wrapper object that gives access to all global variables. You
 *  can use it more or less the same as the $_GLOBALS object in
 * 	PHP.
 * 
 * 	The global PHP variables are acessible via the Php::globals["varname"]
 *  variables.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
 
/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward definitions
 */
class Global;

/**
 *  Class definition
 */
class Globals
{
public:
    /**
     *  Disable copy and move operations
     */
    Globals(const Globals &globals) = delete;
    Globals(Globals &&globals) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Globals() {}
    
    /**
     *  Get access to a global variable
     *  @param  name
     *  @return Global
     */
    Global operator[](const char *name);
    
    /**
     *  Get access to a global variable
     *  @param  name
     *  @return Global
     */
    Global operator[](const std::string &name);
    
    /**
     *  Call a function in PHP
     *  We have ten variants of this function, depending on the number of parameters
     *  @param  name        Name of the function
     *  @return Value
     */
    Value call(const Value &name);
    Value call(const Value &name, Value p0);
    Value call(const Value &name, Value p0, Value p1);
    Value call(const Value &name, Value p0, Value p1, Value p2);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8);
    Value call(const Value &name, Value p0, Value p1, Value p2, Value p3, Value p4, Value p5, Value p6, Value p7, Value p8, Value p9);

private:
    /**
     *  Call function with a number of parameters
     *  @param  name        Function name
     *  @param  argc        Number of parameters
     *  @param  argv        The parameters
     *  @return Value
     */
    Value exec(const Value &name, int argc, struct _zval_struct ***params);

    /**
     *  Constructor
     */
    Globals() {}
    
public:
	/**
	 *  Get the one and only instance
	 *  @return	Globals
	 */
	static Globals &instance();
};

/**
 *  We always have one instance
 *	@var	Globals
 */
extern Globals &globals;

/**
 *  End of namespace
 */
}

