/**
 *
 *  Test variables
 *	019-HashMember-2.phpt
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
	Php::Value test_HashMember_2()
	{
	    Php::Value r, empty_array(Php::Type::Array);
	    r["k1"]["k3"] = "v1";
	    r["k1"]["k2"]["k4"] = "v2";
	    r["k5"][1] = "v3";
	    r[2]["k6"][1] = "v4";
	    r[3][4][1] = "v5";
	    r[3][4][2][5][7][11] = "v5";
	    r[3][4][2][5][7]["k"] = "v5";
	    r["c"][0] = "nested value";
	    r["c"][1] = nullptr;
	    r["c"][2] = empty_array;
	    r["c"][3] = "example";
	    return r;
	}

/**
 *  End of namespace
 */
}

