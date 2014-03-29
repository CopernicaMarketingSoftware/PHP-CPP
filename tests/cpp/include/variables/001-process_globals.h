/**
 *
 *  Test variables
 *	001-process_globals.phpt
 *	Global variables in PHP-CPP
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {

   

	/**
	 *  process_globals()
	 * 
	 *  This function reads and modifies global variables
	 */
	Php::Value process_globals()
	{
	    // all global variables can be accessed via the Php::GLOBALS variable,
	    // which is more or less the same as the PHP $_GLOBALS variable
	    
	    // set a global variable
	    Php::GLOBALS["a"] = 1;
	    
	    // increment a global variable
	    Php::GLOBALS["b"] += 1;
	    
	    // set a global variable to be an array
	    Php::GLOBALS["c"] = Php::Array();
	    
	    // add a member to an array
	    Php::GLOBALS["c"]["member"] = 123;
	    
	    // and increment it
	    Php::GLOBALS["c"]["member"] += 77;
	    
	    // change value e
	    Php::GLOBALS["e"] = Php::GLOBALS["e"][0]("hello");
	    
	    // if a global variable holds a function, we can call it
	    return Php::GLOBALS["d"](1,2,3);
	}



/**
 *  End of namespace
 */
}

