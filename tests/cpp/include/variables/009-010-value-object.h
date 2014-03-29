/**
 *
 *  Test variables
 *	009-value-object.phpt
 *	010-value-object2.phpt
 *
 */


/**
 *  Set up namespace
 */
namespace TestVariables {


	/*
	 * Test Php::Value object
	 */
	Php::Value value_object1(void)
	{

		// create empty object of type stdClass
		Php::Object object;

		// object properties can be accessed with square brackets
		object["property1"] = "value1";
		object["property2"] = "value2";

		// Php::Value is the base class, so you can assign Php::Object objects
		//Php::Value value = object;

		return object;
	}

	/*
	 * Test Php::Value object
	 */
	Php::Value value_object2(void)
	{

		// create empty object of type stdClass
		Php::Object object;

		// to create an object of a different type, pass in the class name 
		// to the constructor with optional constructor parameters
		//object = Php::Object("DateTime", "2014-03-27 00:37:15.638276");

		auto timeZone = Php::Object("DateTimeZone", "Europe/Amsterdam");
		object = Php::Object("DateTime", "2014-03-27 00:37:15", timeZone);


		// methods can be called with the call() method
		Php::out << object.call("format", "Y-m-d H:i:s") << std::endl;

		// all these methods can be called on a Php::Value object too
		Php::Value value = Php::Object("DateTime", "2016-03-31 15:48:00", timeZone);
		Php::out << value.call("format", "Y-m-d H:i:s") << std::endl;
		Php::out << value.call("getOffset") << std::endl;
		
		return object;
	}


/**
 *  End of namespace
 */
}

