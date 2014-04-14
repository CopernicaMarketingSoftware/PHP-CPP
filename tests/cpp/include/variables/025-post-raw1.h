/**
 *
 *  Test superglobal variables _POST
 *      025-post-raw1.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {
    using namespace Php;


    /*
     * Test 
     */
    void post_raw1(void)
    {
        out << "username => "<< POST["username"] << std::endl;
        out << "text => "<< POST["text"] << std::endl;

        /*
        XXX TODO: conversion from ‘Php::Super’ to  ‘Php::Value‘
        Value v = POST;

        out << "Array/Object contains " << v.size() << " items" << std::endl;
        for (auto it=v.begin(), itend = v.end(); it != itend; ++it) {
            out << "["<< it->first << "]="<< it->second << std::endl;
        }
        */
    }

/**
 *  End of namespace
 */
}

