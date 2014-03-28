/**
 *
 *  Test variables
 *	007-overloaded-operators.phpt
 *
 */



namespace TestVariables {

	/*
	 * Test Php::Value overloaded operators
	 */
	void overloaded_op(Php::Parameters &params)
	{
	    Php::Value value = params[0];
	    if (value == "some string")
	    {
	        Php::out  << "value == 'some string'"  << std::endl;
	    }

	    if (value == 12)
	    {
	        Php::out  << "value == 12"  << std::endl;
	    }
	    else if (value > 100)
	    {
	        Php::out  << "value > 100" << std::endl;
	    }

	    value += 10;
	    Php::out << value << std::endl;
	    
	    int r1 = value - 8;
	    Php::out << r1 << std::endl;

	    double r2 = value*123.45;
	    Php::out << r2 << std::endl;

	    double r3 = value/123.45;
	    Php::out << r3 << std::endl;
	}

}

