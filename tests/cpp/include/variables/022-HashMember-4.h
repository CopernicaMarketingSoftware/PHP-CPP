/**
 *
 *  Test variables
 *	019-HashMember-4.phpt
 *	Test HashMember
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {

	/**
	 *  This function returns complex array
	 */
	Php::Value test_HashMember_4()
	{
	    

	    Php::Value r1;
	    Php::Value tmp1,tmp2;
	    tmp2.set("key3", "val");
	    tmp1.set("key2", tmp2);
	    r1.set("key1", tmp1);
		// this should be equivalent to:
    	// r1["key1"]["key2"]["key3"] = "val";

	    Php::Value r2;
	    r2.set("str1", "example");
	    r2.set("str2", r2.get("str1"));
		// this should be equivalent to:
    	// r2["str1"] = "example";
    	// r2["str2"] = r2["str1"];
    	// i.e.
    	// r2["str1"] = "example";
    	// r2["str2"] = "example";

	    Php::Value r3;
	    Php::Value tmp;
	    tmp.set("str2", "val1-2");
	    tmp.set("str3", "val1-3");
	    r3.set("str1", tmp);
		// this should be equivalent to:
    	// r3["str1"]["str2"] = "val1-2";
    	// r3["str1"]["str3"] = "val1-3";


	    Php::Value r;
	    r[0] = r1;
	    r[1] = r2;
	    r[2] = r3;


	    return r;
	}

/**
 *  End of namespace
 */
}

