/**
 *  Globals.h
 *
 *  Wrapper object that gives access to all global variables. You
 *  can use it more or less the same as the $_GLOBALS object in
 *  PHP.
 *
 *  The global PHP variables are acessible via the Php::globals["varname"]
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
class PHPCPP_EXPORT Globals
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

private:
    /**
     *  Constructor
     */
    Globals() {}

public:
    /**
     *  Get the one and only instance
     *  @return Globals
     */
    static Globals &instance();
};

/**
 *  We always have one instance of the GLOBALS instance
 *  @var    Globals
 */
extern PHPCPP_EXPORT Globals &GLOBALS;

/**
 *  End of namespace
 */
}
