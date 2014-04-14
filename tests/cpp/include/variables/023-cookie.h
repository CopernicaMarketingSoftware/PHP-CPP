/**
 *
 *  Test superglobal variables _COOKIE
 *	023-cookie.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {


	/*
	 * Test 
	 */
	void getCookie(void)
	{
        Php::out << "_COOKIE[peace]   = " << Php::COOKIE["peace"] << std::endl;
        Php::out << "_COOKIE[freedom] = " << Php::COOKIE["freedom"] << std::endl;
        Php::out << "_COOKIE[empty]   = " << Php::COOKIE["empty"] << std::endl;
	}

/**
 *  End of namespace
 */
}

