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
        /*
            .add(Php::Ini(, "valIni1"))
            .add(Php::Ini("ini2", "valIni2", "OrigValIni2"))
            .add(Php::Ini("ini3", "valIni3", "OrigValIni3", Php::Ini::System))
            .add(Php::Ini("ini4", true,      false, Php::Ini::Place::User))
            .add(Php::Ini("ini5",            false));

            Ini::get("ini1")
            */

        Php::out << "Ini::get(ini1) = {{" << Php::Ini::get("ini1") << " | " << Php::Ini::get_orig("ini1") << "}}" << std::endl;
        Php::out << "Ini::get(ini2) = {{" << Php::Ini::get("ini2") << " | " << Php::Ini::get_orig("ini2") << "}}" << std::endl;
        Php::out << "Ini::get(ini3) = {{" << Php::Ini::get("ini3") << " | " << Php::Ini::get_orig("ini3") << "}}" << std::endl;
        Php::out << "Ini::get(ini4) = {{" << Php::Ini::get("ini4") << " | " << Php::Ini::get_orig("ini4") << "}}" << std::endl;
        /*
        Php::out << "extension_for_tests.some_string ={{" << Php::ini_get("extension_for_tests.some_string") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string2 ={{" << Php::ini_get("extension_for_tests.some_string2") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string3 ={{" << Php::ini_get("extension_for_tests.some_string3") << "}}" << std::endl;
        Php::out << "extension_for_tests.some_string3 ={{" << Php::ini_get("extension_for_tests.some_bool") << "}}" << std::endl;
        */
        


        return;
    }
/**
 *  End of namespace
 */
}

