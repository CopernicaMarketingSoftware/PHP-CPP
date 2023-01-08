#include <string>
#include <iostream>
#include <unordered_map>
#include <phpcpp.h>

#include "phpinfo.hpp"

/**
 *  Namespace to use
 */
using namespace std;

// NOTE the EXTENSION NAME HERE !
INFO_FUNC(phpinfo) {
    bool asText = static_cast<bool>(Php::constant("PHP_SAPI") == "cli");

    std::unordered_map<std::string, std::string> infoMap;

    // Change info here as needed
    infoMap["Extension Version"]            = MODULE_VERSION;
    infoMap["Author"]                       = MODULE_AUTHOR;
    infoMap["Copyright"]                    = MODULE_COPYRIGHT;

    // Headline
    if (!asText) {
        Php::out << "<h2><a name=\"module_" << MODULE_NAME << "\">" << MODULE_NAME << "</a> Information</h2>\n"
                 << std::endl;
    } else {
        Php::out << MODULE_NAME << " Information" << std::endl;
    }

    // php_info_print_table_start()
    if (!asText) {
        Php::out << "<table>\n" << std::endl;
    } else {
        Php::out << "\n";
    }

    // php_info_print_table_header()
    for (auto &iter : infoMap) {
        if (!asText) {
            Php::out << "<tr>" << std::endl;
            Php::out << "<td class=\"e\">" << iter.first << "</td>" << std::endl;
            Php::out << "<td class=\"v\">" << iter.second<< "</td>" << std::endl;
            Php::out << "</tr>"<< std::endl;
        } else {
            Php::out << iter.first << " => " << iter.second << std::endl;
        }
    }

    // php_info_print_table_end()
    if (!asText) {
        Php::out << "</table>\n" << std::endl;
    }

    // Ini vars
    int iniCount = (int) extension->iniVariables();

    if (iniCount > 0) {
        if (!asText) {
            Php::out << "<table><tr class=\"h\"><th>Directive</th><th>Local Value</th><th>Master Value</th></tr>"
                << std::endl;
        } else {
            Php::out << "\n";
            Php::out << "Directive => Local Value => Master Value" << std::endl;
        }

        extension->iniVariables([asText](Php::Ini &ini) {
            std::string name = ini.name();
            Php::IniValue master = Php::ini_get_orig(name.c_str());
            Php::IniValue local  = Php::ini_get(name.c_str());
            std::string masterVal = master;
            std::string localVal = local;
            masterVal = (masterVal.empty()) ? "no value" : masterVal;
            masterVal = (masterVal == "no value" && !asText) ? "<i>" + masterVal + "</i>" : masterVal;
            localVal = (localVal.empty()) ? "no value" : localVal;
            localVal = (localVal == "no value" && !asText) ? "<i>" + localVal + "</i>" : localVal;

            if (!asText) {
                Php::out << "<tr>" << std::endl;
                Php::out << "<td class=\"e\">" << name  << "</td><td class=\"v\">" << localVal
                    << "</td><td class=\"v\">" << masterVal << "</td>" << std::endl;
                Php::out << "</tr>"<< std::endl;
            } else {
                Php::out << name << " => " << localVal << " => " << masterVal << std::endl;
            }
        });

        if (!asText) {
            Php::out << "</table>\n" << std::endl;
        }
    }

    Php::out << std::flush;
}

// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        // create extension
        static Php::Extension extension(MODULE_NAME_LOWER, MODULE_VERSION);

		// Add a couple of INI settings
		extension.add(Php::Ini(MODULE_NAME_LOWER ".some.value", "", Php::Ini::Place::All));
		extension.add(Php::Ini(MODULE_NAME_LOWER ".some.value2", "", Php::Ini::Place::All));

        /* MODULE INFO - REGISTER A CALLBACK FOR PHPINFO() */
        Php::Extension *extPtr = &extension;

        extension.onInfo([extPtr]() {
		    // NOTE the EXTENSION NAME HERE !
            CALL_INFO_FUNC(phpinfo, extPtr);
        });

        // return the module entry
        return extension.module();
    }
}


