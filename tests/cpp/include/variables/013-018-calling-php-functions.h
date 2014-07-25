/**
 *
 *  Test call function
 *	013-calling-php-functions.phpt
 *	...
 *	018-calling-php-functions.phpt
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
	    param5.set(1, 3.14159265358979);
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
	    param5.set(1, 3.14159265358979);
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

	/*
	 * Test 
	 */
	void fnFromUserSpace2(void)
	{

	    // create an object (this will also call __construct())
	    Php::Object time("DateTime", "2014-03-28 21:22:15", Php::Object("DateTimeZone", "Asia/Irkutsk"));
	    
	    // call a method on the datetime object
	    Php::out << time.call("format", "Y-m-d H:i:s") << std::endl;
	    
	    // in PHP it is possible to create an array with two parameters, the first
	    // parameter being an object, and the second parameter should be the name
	    // of the method, we can do that in PHP-CPP too
	    Php::Array time_format({time, "format"});
	    
	    // call the method that is stored in the array
	    Php::out << time_format("Y-m-d H:i:s") << std::endl;

	    // call method of class from user space
	    Php::Object usrspcl("usrspClass", "Mount Meru");
	    Php::Array usrspcl_meth({usrspcl, "someMethod"});
	    // call the method that is stored in the array
	    Php::out << usrspcl_meth("is in the Arctics") << std::endl;
	    
	    // call callable object of class from user space
	    Php::Object clbl("CallableClass", "Arctics around mount Meru");
	    Php::out << clbl("is the birthplace of the Hyperboreans") << std::endl;
	}

/**
 *  End of namespace
 */
}

