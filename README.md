PHP-CPP
=======

The PHP-CPP library is a C++ library for developing PHP extensions. It offers a collection
of well documented and easy-to-use classes that can be used and extended to build native
extensions for PHP.

Unlike regular PHP extensions - which are really hard to implement and require a deep
knowledge of the Zend engine and pointer manipulation - extensions built with PHP-CPP
are not difficult to developer at all. In fact, the only thing you need to do is write a function in
C++, and the PHP-CPP library uses all the power offered by C++11 to convert the parameters and return
values from your functions to/and from PHP:

<code><pre>
/**
 *  Hello world function
 *  @return std::string
 */
std::string hello_word()
{
    return std::string("hello world!");
}
</pre></code>

The function above is a native C++ function. With PHP-CPP you can export this function
to PHP with only one single C++ method call:

<code><pre>
extension.function("hello_world", hello_world);
</pre></code>

Working with parameters and return values is just as easy:

<code><pre>
/**
 *  Function that takes parameters
 *  @param  int
 *  @param  int
 *  @return int
 */
int my_plus(int a, int b)
{
    return a+b;
}
</pre></code>

The PHP-CPP library ensures that the variables
from PHP (which internally are complicated C structures), are automatically converted into 
integers, passed to your function, and that the return value of your "my_plus" function is 
also converted back into a PHP variable.

Type conversion between native C/C++ types and PHP variables is handled by PHP-CPP, using
features from the C++11 language. It does not matter if your functions accept strings,
integers, booleans or other native parameters: PHP-CPP takes care of the conversion. 
The return value of your function is also transformed by PHP-CPP into PHP.

More complicated structured can be handled by PHP-CPP as well. If you would like to return
a nested associative array from your function, you can do so too:

<code><pre>
/**
 *  A C++ function that returns a nested PHP array
 *  @return array
 */
PhpCpp::Value get_complex_array()
{
    PhpCpp::Value r;
    r["a"] = 123;
    r["b"] = 456;
    r["c"][0] = "nested value";
    r["c"][1] = "example";
    return r;
}
</pre></code>

The C++ function above is equivalent to the following function in PHP:

<code></pre>
/**
 *  PHP alternative
 *  @return array
 */
function get_complex_array()
{
    return array(
        "a" => 123,
        "b" => 456,
        "c" => array("nested_value","example")
    );
}
</pre></code>

However, this library is currently a work in progress, and it is an open
source project. We are looking for people who'd like to contribute to it.

PHP-CPP is an initiative from Copernica BV.

For more information, contact me at emiel.bruijntjes@copernica.com.


Emiel Bruijntjes (1 September 2013)
