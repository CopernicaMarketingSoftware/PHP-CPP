/**
 *
 *  Test variables
 *	002-get_complex_array.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {

	/**
	 *  This function returns complex array
	 */
	Php::Value get_complex_array()
	{
	    Php::Value r;
	    r["a"] = 123;
	    r["b"] = 456;
	    r["c"][0] = "nested value";
	    r["c"][1] = "example";
	    r["c"][2] = 7;
	    return r;
	}


/**
 *  End of namespace
 */
}

