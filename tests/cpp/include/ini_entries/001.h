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

    // will be retrieved at boot extension
    double ini6val = 0.0;

    void iniTest1(Php::Parameters &params)
    {
        Php::out << "ini_get(ini1) = " << Php::ini_get("ini1") << std::endl;
        Php::out << "ini_get(ini2) = " << Php::ini_get("ini2") << std::endl;
        Php::out << "ini_get(ini3) = " << Php::ini_get("ini3") << std::endl;
        Php::out << "ini_get(ini4) = " << Php::ini_get("ini4") << std::endl;
        Php::out << "ini_get(ini5) = " << Php::ini_get("ini5") << std::endl;
        Php::out << "ini_get(ini6) = " << Php::ini_get("ini6") << std::endl;

        Php::out << "ini6val = " << ini6val << std::endl;
    }
/**
 *  End of namespace
 */
}

