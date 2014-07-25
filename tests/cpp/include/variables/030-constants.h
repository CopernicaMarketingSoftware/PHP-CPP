/**
 *
 *  Test superglobal variables _ENV
 *  027-env.phpt
 *
 */



namespace TestVariables {

    
    /*
     * Test 
     */
    void test_constants(void)
    {
        Php::define("MY_TEST_CONSTANT", "hello");
        Php::out << Php::constant("MY_TEST_CONSTANT") << std::endl;
        Php::out << bool2str(Php::defined("MY_TEST_CONSTANT")) << std::endl;
        Php::out << bool2str(Php::defined("NOT_MY_TEST_CONSTANT")) << std::endl;
    }

}

