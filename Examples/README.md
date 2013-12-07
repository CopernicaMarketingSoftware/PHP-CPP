This directory contains a number of examples that show how to use the
PHP-CPP library. 

To run an example, there are a couple of steps that need to be taken. 
The first step is compiling and installing the PHPCPP library. This is
done by running 'make' and then 'make install' in the main directory.

The second step is to compile the C++ code of an example and make it 
into an extension usable by PHP. This is done by running 'make' and 
'make install' in an Example directory. Do make sure you've edited the 
Makefile according to your own specific directories.

The following examples are available:


### [Extension](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/Extension)

    The first example does nothing - it only shows how to create your
    own extension. This means your extension will be listed in the 
    output of "phpinfo()", and it is included in the array returned 
    by theget_loaded_modules() function.
    
    There are no functions or classes defined by this first example
    extension.
    
    
### [FunctionVoid](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/FunctionVoid)

    This second example shows how to add a function to the extension 
    and call that function from the PHP code. Adding a function to 
    your extension means that you can call it anywhere from the PHP 
    code.
    
    Furthermore, it is possible to associate your C++ function with 
    another name. This other name is then used in PHP to call the C++ 
    function, rather than the original C++ function name.
    
    Functions and/or classes defined in this example.
        - void my_function_void() Named as my_void_function()


### [FunctionReturnValue](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/FunctionReturnValue)

    The third example shows how to return a value from C++ to PHP. 
    Virtually any type of value can be returned to PHP from C++.
    The returned value must be returned as Php::Value object, rather 
    than a native C/C++ type. This Php::Value class takes care of
    converting native values into values usable in your PHP code.
    
    Because a Php::Value is always returned, there is no need to specify
    the return type of the function when adding it to your extension.
    
    Functions and/or classes defined in this example.
        - Php::Value my_return_value_function()
        

### [FunctionNoParameters](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/FunctionNoParameters)

    The fourth example is a combination of the second and third example.
    This example illustrates how to call a function without parameters.
    The function is added to your extension, and can then be called from
    your PHP script.
    
    The function returns a Php::Value to show that the call succeeded.
    
    Functions and/or classes defined in this example.
        - Php::Value my_no_parameters_function()
    
    
### [FunctionWithParameters](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/FunctionWithParameters)

    The fifth example is an example to show how several different types
    of parameters can used for functions. There are two ways to pass a 
    parameter, by value(Php::ByVal) and by reference(Php::ByRef). Each 
    take two parameters of their own. The first being the parameter name,
    and the second the parameter type.
    
    Furthermore, parameters are always stored in the Php::Parameters 
    object. This object is basicly an array which hold all the parameters,
    in order.
    
    The first option being the undefined parameters. With undefined 
    parameters, we can pass any and as many parameters as we want to 
    the function. 
    
    The second option is defining each parameter when adding the function
    to your extension. In this case we have added two Php::numericType
    parameters to the function. In 'type.h' you can find all avaiable
    types, however not every type has been implemented yet.
    
    The third option is passing a reference of a variable. Meaning when
    it is altered in the C++ code, its value will also change in the PHP
    code. This can achieved by using Php:ByRef, rather than Php::ByVal.
    
    The fourth option is passing an array as parameter. The array 
    parameter will be accessible from the N-1 index of the 
    Php::Parameters object, where is the argument number of the array
    when passing it to the function.
    
    The fifth and final option is passing an object. An object can be 
    passed in the same way as any other data type, except for that 
    you must specify what the class is of the object. This can be done
    by passing a string with the class name as the second parameter to 
    Php::ByVal or Php::ByRef.
    
    Functions and/or classes defined in this example.
        1. void my_with_undefined_parameters_function(Php::Parameters &params)
        2. Php::Value my_with_defined_parameters_function(Php::Parameters &params)
        3. void my_with_defined_parameters_reference_function(Php::Parameters &params)
        4. void my_with_defined_array_parameters_function(Php::Parameters &params)
        5. void my_with_defined_object_parameters_function(Php::Parameters &params)


### [Globals](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/Globals)

    Global PHP variables can be used accessed from your C++ code. You
    can do this by accessing the Php::values array, which more or less
    behaves the same as the $_GLOBALS array does in PHP.

    Functions and/or classes defined in this example.
        1. void process_globals()


### [Exceptions](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/Exceptions)

    The sixth example is composed of two parts, the throw exception and 
    the catch exception examples. The requirements of the catch example,
    passing a callback as a parameter, have not yet been implemented. 
    
    The throw example is there to show that an exception thrown in
    a C++ function can be caught and handled in your PHP script. The 
    exception thrown is a Php::Exception.
    
    The catch example shows that when a PHP function is passed as a 
    callback, and is capable of throwing a (PHP) exception, that it
    can be caught as Php::Exception and then handled in the C++ code. 
    However, the passing of a function as a callback has not yet been 
    implemented. It would need to be implemented for this specific 
    example to work.
    
    Functions and/or classes defined in this example.
        - void my_catch_exception_function(Php::Parameters &params)
        - void my_throw_exception_function()


### [PHP function calls](https://github.com/EmielBruijntjes/PHP-CPP/tree/master/Examples/CallPhpFunctions)

    The seventh example shows how to pass a callable PHP function as
    a parameter. As can be seen in the example, there are several ways 
    of passing a PHP function to the C++ function. When a function is 
    passed, it is possible to use the () operator on the parameter, with 
    the correct amount of parameters for the callable PHP function.
    
    When using the wrong amount, or when trying to use the () operator
    on a non-callable type, you will get PHP errors rather than 
    segmentation faults or other kinds of C++ errors.
    
    Functions and/or classes defined in this example.
        - Php::Value call_php_function(Php::Parameters &params)

