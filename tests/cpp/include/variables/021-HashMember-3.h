/**
 *
 *  Test variables
 *	019-HashMember-3.phpt
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
	Php::Value test_HashMember_3()
	{
	    
	    Php::Value r, tmp(Php::Type::Array);
	    //Php::Value tmp;

		tmp.set("key2", "val1-2");
		r.set("key1", tmp);
		r.get("key1").set("key3", "val1-3");

		// expect to receive the same as when recording:
		//r["key1"]["key2"] = "val1-2";
	    //r["key1"]["key3"] = "val1-3";
	    
		return r;
	}

/**
 *  End of namespace
 */
}

