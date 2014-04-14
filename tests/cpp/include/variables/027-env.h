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
    void test_env(void)
    {
        Php::out << "HTTP_USER_AGENT => " << Php::SERVER["HTTP_USER_AGENT"] << std::endl;
        Php::out << "ENVVAR1 => " << Php::SERVER["ENVVAR1"] << std::endl;
        Php::out << "HTTP_REFERER => " << Php::SERVER["HTTP_REFERER"] << std::endl;
        Php::out << "REQUEST_METHOD => " << Php::SERVER["REQUEST_METHOD"] << std::endl;
        Php::out << "HTTP_HOST => " << Php::SERVER["HTTP_HOST"] << std::endl;
    }

}

