This directory contains a number of examples that show how to use the
PHP-CPP library. 

To run an example, there are several steps which need to be taken. 
The first step is compiling and installing the PHPCPP library. This is
done running 'make' and then 'make install' in the main directory.
The second step is compiling the C++ code and made into an extension 
usable by PHP. This is done by running 'make' and 'make install' 
in an Example directory. Do make sure you've edited the Makefile
according to your own specific directories.

The following examples are available:


Extension
~~~~~~~~~

	The first example does nothing - it only shows how to create your
	own extension. This means your extension will be listed in the 
	output of "phpinfo()", and it is included in the array returned 
	by theget_loaded_modules() function.
	
	There are no functions or classes defined by this first example
	extension.
	
	
FunctionVoid
~~~~~~~~~~~~

	This second example shows how to add a function to the extension 
	and call that function from the PHP code. Adding a function to 
	your extension means that you can call it anywhere from the PHP 
	code.
	
	Furthermore, it is possible to associate your C++ function with 
	another name. This other name is then used in PHP to call the C++ 
	function, rather than the original C++ function name.
	
	Functions and/or classes defined in this example.
		- void my_function_void() Named as my_void_function()
	
FunctionReturnValue
~~~~~~~~~~~~~~~~~~~

	The third example shows how to return a value from C++ to PHP. 
	Virtually any type of value can be returned to PHP from C++.
	The returned value must be returned as Php::Value, rather than
	its own type. This Php::Value can then be used in your PHP code.
	
	Because a Php::Value is always returned, there is no need to specify
	the return type of the function when adding it to your extension.
	
	Functions and/or classes defined in this example.
		- Php::Value my_return_value_function()
		
FunctionNoParameters
~~~~~~~~~~~~~~~~~~~~

	The fourth example is a combination of the second and third example.
	This example illustrates how to call a function without parameters.
	The function is added to your extension, and can then be called from
	your PHP script.
	
