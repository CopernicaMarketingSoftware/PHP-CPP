/**
 *  Simple.h
 *
 *  A very simple extension that does almost nothing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include <string>
#include <iostream>
#include <unordered_map>
#include <phpcpp.h>

#include "phpinfoexample.hpp"

/**
 *  Namespace to use
 */
using namespace std;


Php::Value bubblesort(Php::Parameters &params)
{
    cout << "start bubblesort" << endl;
    
    // the array that was passed
    Php::Value array(params[0]);
    
    cout << "go return" << endl;
    
    return array;
    
    // size of the array
    int size = array.size();
    
    cout << "convert to native" << endl;
    
    int *x = new int[size];
    for (int i=0; i<size; i++) x[i] = array[i];
    
    cout << "converted" << endl;
    
    
    // outer loop
    for (int i=0; i<size; i++)
    {
        // left value
        int left = x[i];
//        cout << "left: " << left << endl;
        
        // inner loop
        for (int j=i+1; j<size; j++)
        {
            // right value
            int right = x[j];
            
            if (left < right) continue;
            
            // swap values
            x[i] = right;
            x[j] = left;
            left = right;
        }
    }
    
    cout << "algorithm end" << endl;
    
    Php::Value r;
    
    for (int i=0; i<size; i++) r[i] = x[i];
    
    
    delete[] x;
    
    // done
    return r;
}



/**
 *  Our own "my_plus" function that will be available in PHP
 *  @param  environment
 *  @param  params
 *  @return Value
 */
static Php::Value my_plus(Php::Parameters &params)
{
    Php::Value r(0);
    
    for (unsigned int i=0; i<params.size(); i++) r += params[i];
    
    return r;
    
    
//    int p1 = params[0];
//    int p2 = params[1];
//    return p1 + p2;
//    
//    cout << "global g1: " << env["g1"].stringValue() << endl;
//    cout << "global g2: " << env["g2"].stringValue() << endl;
//    
//    Php::Global g(env["g3"]);
//    
//    g = "zo kan het ook";
//    
//    string output = env.call("strtoupper","test in lowercase");
//    
//    cout << "output: " << output << endl;
//    
//    return p1 + p2;
}

/**
 *  Custom class that will be available in PHP
 */
class MyCustomClass : public Php::Base
{
private:
    int _x;
    
public:
    MyCustomClass()
    {
        _x = 3;
        cout << "MyCustomClass::MyCustomClass" << endl;
        cout << this << endl;
        cout << _x << endl;
    }
    
    virtual ~MyCustomClass()
    {
        cout << "MyCustomClass::~MyCustomClass" << endl;
    }

    virtual void __construct()
    {
        cout << "MyCustomClass::__construct" << endl;
    }

    virtual void __destruct()
    {
        cout << "MyCustomClass::__destruct" << endl;
    }
    
    void myMethod(Php::Parameters &params)
    {
        cout << "myMethod GETS CALLED!!!!" << endl;
        cout << this << endl;
        cout << _x << endl;
//        cout << "A: " << _properties["a"] << endl;
//        cout << "Z: " << _properties["z"] << endl;
    }
};

// NOTE the EXTENSION NAME HERE !
INFO_FUNC(phpinfoexample) {

    bool isCLI = (bool) (Php::constant("PHP_SAPI") == "cli");

    std::unordered_map<std::string, std::string> infoMap;

    // Change info here as needed
    infoMap["Extension Version"]            = MODULEVERSION;
    infoMap["Author"]                       = MODULE_AUTHOR;
    infoMap["Copyright"]                    = MODULE_COPYRIGHT;

    // Headline
    if (!isCLI)
        Php::out << "<h2><a name=\"module_" << MODULENAME << "\">" << MODULENAME << "</a> Information</h2>\n" << std::endl;
    else
        Php::out << MODULENAME << " Information" << std::endl;

    // php_info_print_table_start()
    if (!isCLI)
        Php::out << "<table>\n" << std::endl;
    else
        Php::out << "\n";

    // php_info_print_table_header()
    for (auto &iter : infoMap) {
        if (! isCLI) {
            Php::out << "<tr>" << std::endl;
            Php::out << "<td class=\"e\">" << iter.first << "</td>" << std::endl;
            Php::out << "<td class=\"v\">" << iter.second<< "</td>" << std::endl;
            Php::out << "</tr>"<< std::endl;
        } else {
            Php::out << iter.first << " => " << iter.second << std::endl;
        }
    }

    // php_info_print_table_end()
    if (! isCLI)
        Php::out << "</table>\n" << std::endl;
    else
        Php::out << "\n";

    // Ini vars
    int iniCount = (int) extension->iniVariables();

    if (iniCount > 0) {

        if (!isCLI) {
            Php::out << "Directive => Local Value => Master Value" << std::endl;
        } else {
            Php::out << "<table><tr class=\"h\"><th>Directive</th><th>Local Value</th><th>Master Value</th></tr>" << std::endl;
        }

        extension->iniVariables([isCLI](Php::Ini &ini) {

            std::string name = ini.name();
            Php::IniValue master = Php::ini_get_orig(name.c_str());
            Php::IniValue local  = Php::ini_get(name.c_str());
            std::string masterVal = master;
            std::string localVal = local;
            masterVal = (masterVal.empty()) ? "no value" : masterVal;
            masterVal = (masterVal == "no value" && !isCLI) ? "<i>" + masterVal + "</i>" : masterVal;
            localVal = (localVal.empty()) ? "no value" : localVal;
            localVal = (localVal == "no value" && !isCLI) ? "<i>" + localVal + "</i>" : localVal;

            if (! isCLI) {
                Php::out << "<tr>" << std::endl;
                Php::out << "<td class=\"e\">" << name  << "</td><td class=\"v\">" << localVal << "</td><td class=\"v\">" << masterVal << "</td>" << std::endl;
                Php::out << "</tr>"<< std::endl;
            } else {
                Php::out << name << " => " << localVal << " => " << masterVal << std::endl;
            }
        });

        // php_info_print_table_end()
        if (! isCLI)
            Php::out << "</table>\n" << std::endl;
        else
            Php::out << "\n";
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
        static Php::Extension extension(MODULENAME_LOWER, MODULEVERSION);

        // define the functions
        extension.add<my_plus>("my_plus", {
            Php::ByVal("a", Php::Type::Numeric),
            Php::ByVal("b", Php::Type::Numeric),
            Php::ByVal("c", "MyClass"),
            Php::ByRef("d", Php::Type::String)
        });
        
        extension.add<bubblesort>("bubblesort");
        
        // define classes
        Php::Class<MyCustomClass> myCustomClass("my_class");
        myCustomClass.method<&MyCustomClass::myMethod>("mymethod");
        myCustomClass.method<&MyCustomClass::__construct>("__construct");
        
        // add to extension
        extension.add(myCustomClass);
       
		// Add a couple of INI settings
		extension.add(Php::Ini(MODULENAME_LOWER ".some.value", "", Php::Ini::Place::All));
		extension.add(Php::Ini(MODULENAME_LOWER ".some.value2", "", Php::Ini::Place::All));

        /* MODULE INFO - REGISTER A CALLBACK FOR PHPINFO() */
        Php::Extension *extPtr;
        extPtr = &extension;
        extension.onInfo([extPtr]() {

		    // NOTE the EXTENSION NAME HERE !
            CALL_INFO_FUNC(phpinfoexample, extPtr);
        });

        // return the module entry
        return extension.module();
    }
}


