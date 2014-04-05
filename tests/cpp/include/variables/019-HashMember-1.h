/**
 *
 *  Test variables
 *	019-HashMember-1.phpt
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
	Php::Value test_HashMember_1()
	{
	    Php::Value r, tmp(Php::Type::Array);
	    r["key1"] = tmp;
	    r["key1"]["key2"] = "val2";
	    r["key1"]["key3"] = "val3";
	    return r;
	}

/**
 *  End of namespace
 */
}

