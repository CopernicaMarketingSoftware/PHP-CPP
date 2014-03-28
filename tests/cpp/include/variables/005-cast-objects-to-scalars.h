/**
 *
 *  Test variables
 *	005-cast-objects-to-scalars.phpt
 *
 */



/**
 *  Set up namespace
 */
namespace TestVariables {

	/**
	 *  A sample class, with methods to cast objects to scalars
	 */
	class Obj2Scalar : public Php::Base
	{
	public:
	    /**
	     *  C++ constructor and C++ destructpr
	     */
	    Obj2Scalar() {}
	    virtual ~Obj2Scalar() {}

	    /**
	     *  Cast to a string
	     *
	     *  Note that now we use const char* as return value, and not Php::Value.
	     *  The __toString function is detected at compile time, and it does
	     *  not have a fixed signature. You can return any value that can be picked
	     *  up by a Php::Value object.
	     *
	     *  @return const char *
	     */
	    const char *__toString()
	    {
	        return "Mount Meru, also called Sumeru (Sanskrit)";
	    }
	    
	    /**
	     *  Cast to a integer
	     *  @return long
	     */
	    long __toInteger()
	    {
	        return 27032014;
	    }
	    
	    /**
	     *  Cast to a floating point number
	     *  @return double
	     */
	    double __toFloat()
	    {
	        return 3.14159265359;
	    }
	    
	    /**
	     *  Cast to a boolean
	     *  @return bool
	     */
	    bool __toBool()
	    {
	        return true;
	    }
	};


/**
 *  End of namespace
 */
}

