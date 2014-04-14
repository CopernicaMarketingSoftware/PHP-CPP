/**
 *
 *  Test superglobal variables _GET & _POST
 *      024-get-post.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {


    /*
     * Test 
     */
    void get_post(void)
    {
        Php::out << "_GET[a] = " << Php::GET["a"] << std::endl;
        Php::out << "_GET[b] = " << Php::GET["b"] << std::endl;
        Php::out << "_GET[ar][elm1] = " << Php::GET["ar"]["elm1"] << std::endl;
        Php::out << "_GET[ar][elm2] = " << Php::GET["ar"]["elm2"] << std::endl;

        Php::out << "_POST[c]   = " << Php::POST["c"] << std::endl;
        Php::out << "_POST[d]   = " << Php::POST["d"] << std::endl;
        Php::out << "_POST[e]   = " << Php::POST["e"] << std::endl;
        Php::out << "_POST[e][0]   = " << Php::POST["e"][0] << std::endl;
        Php::out << "_POST[e][1]   = " << Php::POST["e"][1] << std::endl;
    }

/**
 *  End of namespace
 */
}

