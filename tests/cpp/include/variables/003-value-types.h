/**
 *
 *  Test variables
 *      003-value-types.phpt
 *
 */

namespace TestVariables {

    /*
     * Check type of value
     * @param array
     */
    void value_types(Php::Parameters &params) 
    {
        if (params.size() == 0) return;
        Php::Value arr = params[0];
        
        Php::out << "Null: "              << bool2str( arr.get("Null").isNull() ) << std::endl;
        Php::out << "Numeric: "           << bool2str( arr.get("Numeric").isNumeric()) << std::endl;
        Php::out << "Float: "             << bool2str( arr.get("Float").isFloat() ) << std::endl;
        Php::out << "Bool: "              << bool2str( arr.get("Bool").isBool() ) << std::endl;
        Php::out << "Array: "             << bool2str( arr.get("Array").isArray() ) << std::endl;
        Php::out << "Object: "            << bool2str( arr.get("Object").isObject()  ) << std::endl;
        Php::out << "String: "            << bool2str( arr.get("String").isString() ) << std::endl;
        Php::out << "Resource: "          << bool2str( arr.get("Resource").type() == Php::Type::Resource ) << std::endl;
        Php::out << "Constant: "          << bool2str( arr.get("Constant").type() == Php::Type::Constant ) << std::endl;
        Php::out << "ConstantArray: "     << bool2str( arr.get("ConstantArray").type() == Php::Type::ConstantArray ) << std::endl;
        Php::out << "Callable1: "         << bool2str( arr.get("Callable1").isCallable() ) << std::endl;
        Php::out << "Callable2: "         << bool2str( arr.get("Callable2").isCallable() ) << std::endl;
        Php::out << "Callable3: "         << bool2str( arr.get("Callable3").isCallable() ) << std::endl;
        Php::out << "Callable4: "         << bool2str( arr.get("Callable4").isCallable() ) << std::endl;

    }

}

