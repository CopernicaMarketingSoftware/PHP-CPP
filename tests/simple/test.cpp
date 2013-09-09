#include <iostream>
#include <functional>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

/**
 *  Example function that adds numbers
 *  @param	a	A number
 *  @param	b	A different number
 * 	@return		Sum of both parameters
 */
int my_plus(int a, int b)
{
	cout << "my_plus(" << a << ", " << b << ")" << endl;

	return a + b;
}

/**
 *  Another example function that concatenates strings
 *  @param	a	A text
 *  @param	b	A different text
 *  @return		Concattenation
 */
const string my_concat(const string &a, const string &b)
{
	cout << "my_concat(" << a << ", " << b << ")" << endl;
	
	return a + b;
}

/**
 *  Value class that can automatically convert objects between many
 *  different types
 */ 
class Value
{
private:
	/**
	 *  Internal representation
	 *	@var _value
	 */
	string _value;

public:
	/**
	 *  Constructor based on string
	 *  @param	text
	 */
	Value(const string &text) : _value(text) {}

	/**
	 *  Constructor based on string
	 *  @param	text
	 */
	Value(const char *text) : _value(text) {}
	
	/**
	 *  Constructor based on int
	 *  @param	integer
	 */
	Value(int value)
	{
		ostringstream s;
		s << value;
		_value = s.str();
	}
	
	/**
	 *  Destructor
	 */
	virtual ~Value() {}
	
	/**
	 *  Cast to integer
	 *  @return		Numeric representation
	 */
	operator int ()
	{
		return atoi(_value.c_str());
	}

	/**
	 *  Cast to string
	 *  @return		String representation
	 */
	operator const string& ()
	{
		return _value;
	}
};

/**
 *  Simple wrapper around a function
 */
class Function
{
private:
	/**
	 *  The actual function
	 * 
	 * 	Note: 	there must be a much smarter way to achieve the same thing,
	 * 			we just want to have a single member that holds the function
	 * 
	 *  @var	_function
	 */
	union F {
		// union members
		function<Value()> r0;
		function<Value(Value&)> r1;
		function<Value(Value&,Value&)> r2;
		function<void()> v0;
		function<void(Value&)> v1;
		function<void(Value&,Value&)> v2;
		
		// constructors
		F() {}
		F(function<Value()> &f) { r0 = f; }
		F(function<Value(Value&)> &f) { r1 = f; }
		F(function<Value(Value&,Value&)> &f) { r2 = f; }
		F(function<void()> &f) { v0 = f; }
		F(function<void(Value&)> &f) { v1 = f; }
		F(function<void(Value&,Value&)> &f) { v2 = f; }
		
		// destructor
		~F();
		
		// assignment operator
//		F &operator=(function<Value()> &f) { r0 = f; return *this; }
//		F &operator=(function<Value(Value&)> &f) { r1 = f; return *this; }
//		F &operator=(function<Value(Value&,Value&)> &f) { r2 = f; return *this; }
		
	} _function;
	
	/**
	 *  Is the object in a valid state
	 *  @var	bool
	 */
	bool _valid;
	
	/**
	 *  The number of arguments that the function required
	 *  @var	_argc
	 */
	int _argc;
	
	/**
	 *  Does the function return something?
	 *  @var	_return;
	 */
	bool _return;
	
public:
	/**
	 *  Contructor
	 *  @param	f
	 */
	Function(function<Value()> &f) : _function(f)
	{
		_argc = 0;
		_return = true;
		_valid = true;
	}
	
	/**
	 *  Contructor
	 *  @param	f
	 */
	Function(function<Value(Value&)> &f) : _function(f)
	{
		_argc = 1;
		_return = true;
	}

	/**
	 *  Contructor
	 *  @param	f
	 */
	Function(function<Value(Value&,Value&)> &f) : _function(f)
	{
		_argc = 2;
		_return = true;
	}
	
	/**
	 *  Move constructor
	 *  @param	f
	 */
	Function(const Function &&f)
	{
		// copy params
		_argc = f._argc;
		_return = f._return;
		
		switch (_argc) {
		case 0: _function.r0 = f._function.r0; break;
		case 1: _function.r1 = f._function.r1; break;
		case 2: _function.r2 = f._function.r2; break;
		}
	} 
	
	/**
	 *  Destructor
	 */
	virtual ~Function() {}
	
	/**
	 *  Operator to call the function
	 *  @param	args
	 */
	void operator()(vector<Value> args)
	{
		switch (_argc) {
		case 0:	_function.r0();
		case 1: _function.r1(args[0]);
		case 2: _function.r2(args[0], args[1]);
		}
	}
};

/**
 *  Class that stores pointers to functions
 */
class Functions
{
private:
	/**
	 *  Map of functions
	 * 	@var map
	 */
	map<string,Function> _functions;

public:
	/**
	 *  Constructor
	 */
	Functions() {}
	
	/**
	 *  Destructor
	 */
	virtual ~Functions() {}
	
	/**
	 *  Add a function
	 * 	@param	name		The function name
	 * 	@param	func		The function to call
	 */
	void add(const string &name, function<Value(Value&,Value&)> f)
	{
		_functions.insert(pair<string,Function>(name, Function(f)));
	}
	
	/**
	 *  Call the functions
	 *  @param	args
	 */
	Value operator()(vector<Value> args)
	{
		for (auto it = _functions.begin(); it != _functions.end(); it++)
		{
			it->second(args);
		}
		
		return Value(123);
	}

};

/**
 *  Main function
 * 	@param	argc
 *  @param	argv
 */
int main(int argc, char *argv[])
{
	// create a vector of arguments
	vector<Value> arguments;
	
	// fill the arguments
	for (int i=1; i<argc-1; i++) arguments.push_back(argv[i]);
	
	// now register all functions
	Functions functions;
	functions.add("plus", my_plus);
	functions.add("concat", my_concat);
	
	// call all functions
	functions(arguments);
	
	// done
	return 0;
}

