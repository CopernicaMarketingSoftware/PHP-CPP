/**
 *
 *  Test variables
 *	004-store-scalar-variables.phpt
 *
 */

/**
 *  Set up namespace
 */
namespace TestVariables {

	/*
	 * Test variables defined in PHP-CPP
	 */
	Php::Value scalar_store(void) {

		Php::Value value1 = 1234;
		Php::Value value2 = "this is a string";
		Php::Value value3 = std::string("another string");
		Php::Value value4 = nullptr;
		Php::Value value5 = 123.45;
		Php::Value value6 = true;

    	Php::Value r;
    	r[0] = value1;
    	r[1] = value2;
    	r[2] = value3;
    	r[3] = value4;
    	r[4] = value5;
    	r[5] = value6;

    	r[6]  = 1234;
    	r[7]  = "this is a string";
    	r[8]  = std::string("another string");
    	r[9]  = nullptr;
    	r[10] = Php::Value();
    	r[11] = 123.45;
    	r[12] = false;

	    return r;
	}

/**
 *  End of namespace
 */
}

