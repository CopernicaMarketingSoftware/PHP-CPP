/**
 *
 *  Test variables
 *	008-value-arrays.phpt
 *
 */


namespace TestVariables {


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


}

