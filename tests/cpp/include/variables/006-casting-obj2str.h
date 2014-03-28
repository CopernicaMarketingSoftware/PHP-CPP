/**
 *
 *  Test variables
 *	006-casting-obj2str.phpt
 *
 */



/**
 *  Set up namespace
 */
namespace TestVariables {


	/*
	 * Test Php::Value casting operators
	 */
	void value_cast2str(Php::Parameters &params)
	{
	    std::string value = params[0];
	    Php::out  << value  << std::endl;
	}


/**
 *  End of namespace
 */
}

