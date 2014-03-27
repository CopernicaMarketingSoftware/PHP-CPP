/**
 *
 *  Test variables
 *
 */

 #include "doubl2str.h"

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


	std::string bool2str(bool b)
	{
		return b ? "Yes" : "No";
	}

	/*
	 * Check type of value
	 * @param array
	 */
	void value_types(Php::Parameters &params) {
	    if (params.size() == 0) {
	        return;
	    }

        Php::Value arr = params[0];

        Php::out << "Null: "     	  << bool2str( arr.get("Null").type() == Php::Type::Null ) << std::endl;
        Php::out << "Numeric: "  	  << bool2str( arr.get("Numeric").type() == Php::Type::Numeric ) << std::endl;
        Php::out << "Float: "    	  << bool2str( arr.get("Float").type() == Php::Type::Float ) << std::endl;
        Php::out << "Bool: "     	  << bool2str( arr.get("Bool").type() == Php::Type::Bool ) << std::endl;
        Php::out << "Array: "    	  << bool2str( arr.get("Array").type() == Php::Type::Array ) << std::endl;
        Php::out << "Object: "   	  << bool2str( arr.get("Object").type() == Php::Type::Object ) << std::endl;
        Php::out << "String: "   	  << bool2str( arr.get("String").type() == Php::Type::String ) << std::endl;
        Php::out << "Resource: " 	  << bool2str( arr.get("Resource").type() == Php::Type::Resource ) << std::endl;
        Php::out << "Constant: " 	  << bool2str( arr.get("Constant").type() == Php::Type::Constant ) << std::endl;
        Php::out << "ConstantArray: " << bool2str( arr.get("ConstantArray").type() == Php::Type::ConstantArray ) << std::endl;
        Php::out << "Callable1: " 	  << bool2str( arr.get("Callable1").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable2: " 	  << bool2str( arr.get("Callable2").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable3: " 	  << bool2str( arr.get("Callable3").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable4: " 	  << bool2str( arr.get("Callable4").type() == Php::Type::Callable ) << std::endl;
	}

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


	/*
	* Test Php::Value casting operators
	*/
	void value_casting(Php::Parameters &params)
	{
		Php::Value value = params[0];

		long value1 = value;
		std::string value2 = value;
		bool value4 = value;

		Php::out << "   long:" << value1 << "\n string:" << value2 << "\n   bool:" << bool2str(value4) << std::endl;
	}


	/*
	* Test Php::Value casting operators
	*/
	void value_cast2double(Php::Parameters &params)
	{
		Php::Value value = params[0];
		double value3 = value;

		/*
		 * The remark (from valmat).
		 * Somehow std::to_string truncates the tail of numbers of type `double` when converting it to a string.
		 * So I wrote my own function `double2str()`, which does not have this drawback.
		 */
		Php::out << double2str(value3) << std::endl;
	}

	/*
	 * Test Php::Value casting operators
	 */
	void value_cast2str(Php::Parameters &params)
	{
	    std::string value = params[0];
	    Php::out  << value  << std::endl;
	}

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

	/*
	 * Test Php::Value arrays
	 */
	Php::Value value_arrays(void)
	{
		// create a regular array
		Php::Value array;
		array[0] = "apple";
		array[1] = "banana";
		array[2] = "tomato";

		// an initializer list can be used to create a filled array 
		Php::Value filled({ "a", "b", "c", "d"});

		// create an associative array
		Php::Value assoc;
		assoc["apple"] = "green";
		assoc["banana"] = "yellow";
		assoc["tomato"] = "green";

		// the variables in an array do not all have to be of the same type
		Php::Value assoc2;
		assoc2["x"] = "info@example.com";
		assoc2["y"] = nullptr;
		assoc2["z"] = 123;

		// nested arrays are possible too
		Php::Value assoc3;
		assoc3["x"] = "info@example.com";
		assoc3["y"] = nullptr;
		assoc3["z"][0] = "a";
		assoc3["z"][1] = "b";
		assoc3["z"][2] = "c";

		Php::Value r;
		r["array"]  = array;
		r["filled"] = filled;
		r["assoc"]  = assoc;
		r["assoc2"] = assoc2;
		r["assoc3"] = assoc3;
		return r;
	}

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

	/*
	 * Test Php::Value resize
	 */
	void ghfghfhf2(Php::Parameters &params)
	{


	}







/**
 *  End of namespace
 */
}

