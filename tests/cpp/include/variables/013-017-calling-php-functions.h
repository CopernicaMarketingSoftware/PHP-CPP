/**
 *
 *  Test call function
 *	013-calling-php-functions.phpt
 *	...
 *	017-calling-php-functions.phpt
 *
 */


/**
 *  Set up namespace
 */
namespace TestVariables {


	/*
	 * Test call function from user space
	 */
	void fnFromUserSpace(void)
	{


	    Php::out << "fnFromUserSpace" << std::endl;
	    
	    Php::Value param5;
	    param5.set(0, "param5");
	    param5.set(1, 3.14159265359);
	    param5.set(2, 28032014);
	    param5.set("key", "value");

		auto timeZone     = Php::Object("DateTimeZone", "Asia/Yekaterinburg");
		Php::Value param6 = Php::Object("DateTime", "2014-03-28 19:42:15", timeZone);
			    
	    // call a function from user space
	    Php::call("some_function", "param1");
	    Php::call("some_function", "param1", "param2");
	    Php::call("some_function", "param1", "param2", "param3");
	    Php::call("some_function", "param1", "param2", "param3", "param4");
	    Php::call("some_function", "param1", "param2", "param3", "param4", param5);
	    Php::call("some_function", "param1", "param2", "param3", "param4", param5, param6);
	    Php::call("some_function", "param1", "param2", "param3", "param4", "param5", "param6", "param7");
	    Php::call("some_function", "param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8");
	    Php::call("some_function", "param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8", "param9");
	    Php::call("some_function", "param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8", "param9", "param10");

	}

	/*
	 * Test call callback
	 */
	void fnCallback(Php::Parameters &params)
	{
	    Php::out << "call callback" << std::endl;

		Php::Value callback = params[0];

	    
	    Php::Value param5;
	    param5.set(0, "param5");
	    param5.set(1, 3.14159265359);
	    param5.set(2, 28032014);
	    param5.set("key", "value");

		auto timeZone     = Php::Object("DateTimeZone", "Asia/Yekaterinburg");
		Php::Value param6 = Php::Object("DateTime", "2014-03-28 19:42:15", timeZone);
			    
	    // call a function from user space
	    callback("param1");
	    callback("param1", "param2");
	    callback("param1", "param2", "param3");
	    callback("param1", "param2", "param3", "param4");
	    callback("param1", "param2", "param3", "param4", param5);
	    callback("param1", "param2", "param3", "param4", param5, param6);
	    callback("param1", "param2", "param3", "param4", "param5", "param6", "param7");
	    callback("param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8");
	    callback("param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8", "param9");
	    callback("param1", "param2", "param3", "param4", "param5", "param6", "param7", "param8", "param9", "param10");

	}


/**
 *  End of namespace
 */
}

