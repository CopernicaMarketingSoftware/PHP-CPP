/**
 *
 *  Test variables
 *	003-value-types.phpt
 *
 */


namespace TestVariables {

	/*
	 * Check type of value
	 * @param array
	 */
	void value_types(Php::Parameters &params) {
	    if (params.size() == 0) {
	        return;
	    }

        Php::Value arr = params[0];

        Php::out << "Null: "     	  << bool2str( arr.get("Null").type() == Php::Type::Null ) << std::endl;
        Php::out << "Numeric: "  	  << bool2str( arr.get("Numeric").type() == Php::Type::Numeric ) << std::endl;
        Php::out << "Float: "    	  << bool2str( arr.get("Float").type() == Php::Type::Float ) << std::endl;
        Php::out << "Bool: "     	  << bool2str( arr.get("Bool").type() == Php::Type::Bool ) << std::endl;
        Php::out << "Array: "    	  << bool2str( arr.get("Array").type() == Php::Type::Array ) << std::endl;
        Php::out << "Object: "   	  << bool2str( arr.get("Object").type() == Php::Type::Object ) << std::endl;
        Php::out << "String: "   	  << bool2str( arr.get("String").type() == Php::Type::String ) << std::endl;
        Php::out << "Resource: " 	  << bool2str( arr.get("Resource").type() == Php::Type::Resource ) << std::endl;
        Php::out << "Constant: " 	  << bool2str( arr.get("Constant").type() == Php::Type::Constant ) << std::endl;
        Php::out << "ConstantArray: " << bool2str( arr.get("ConstantArray").type() == Php::Type::ConstantArray ) << std::endl;
        Php::out << "Callable1: " 	  << bool2str( arr.get("Callable1").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable2: " 	  << bool2str( arr.get("Callable2").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable3: " 	  << bool2str( arr.get("Callable3").type() == Php::Type::Callable ) << std::endl;
        Php::out << "Callable4: " 	  << bool2str( arr.get("Callable4").type() == Php::Type::Callable ) << std::endl;
	}

}

