/**
 *
 *  Test variables
 *	011-value-casting-operators.phpt
 *	012-value-casting-operators-double.phpt
 *
 */



/**
 *  Set up namespace
 */
namespace TestVariables {

	/*
	* Test Php::Value casting operators
	*/
	void value_casting(Php::Parameters &params)
	{
		Php::Value value = params[0];

		long value1 = value;
		std::string value2 = value;
		bool value4 = value;

		Php::out << "   long:" << value1 << "\n string:" << value2 << "\n   bool:" << bool2str(value4) << std::endl;
	}


	/*
	* Test Php::Value casting operators
	*/
	void value_cast2double(Php::Parameters &params)
	{
		Php::Value value = params[0];
		double value3 = value;

		/*
		 * The remark (from valmat).
		 * Somehow std::to_string truncates the tail of numbers of type `double` when converting it to a string.
		 * So I wrote my own function `double2str()`, which does not have this drawback.
		 */
		Php::out << double2str(value3) << std::endl;
	}


/**
 *  End of namespace
 */
}

