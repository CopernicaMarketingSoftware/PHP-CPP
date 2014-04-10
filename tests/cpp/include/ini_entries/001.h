/**
 *
 *  Test ini entries
 *	test ini_entries/001.phpt
 *
 */

/**
 *  Set up namespace
 */
namespace TestIniEntries {

    
    void iniTest1(Php::Parameters &params)
    {
        Php::out << "extension_for_tests.some_string ={{" << Php::ini_get("extension_for_tests.some_string") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string2 ={{" << Php::ini_get("extension_for_tests.some_string2") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string3 ={{" << Php::ini_get("extension_for_tests.some_string3") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string3 ={{" << Php::ini_get("extension_for_tests.some_bool") << "}}" << std::endl;
        


        return;
    }
/**
 *  End of namespace
 */
}

