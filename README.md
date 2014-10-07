PHP-CPP
=======

The PHP-CPP library is a C++ library for developing PHP extensions. It offers a collection
of well documented and easy-to-use classes that can be used and extended to build native
extensions for PHP. The full documentation can be found on http://www.php-cpp.com.

Unlike regular PHP extensions - which are really hard to implement and require a deep
knowledge of the Zend engine and pointer manipulation - extensions built with PHP-CPP
are not difficult to develop at all. In fact, the only thing you need to do is write a function in
C++, and the PHP-CPP library uses all the power offered by C++11 to convert the parameters and return
values from your functions to/and from PHP:

```c
Php::Value hello_world()
{
    return "hello world!";
}
```

The function above is a native C++ function. With PHP-CPP you can export this function
to PHP with only one single C++ method call:

```c
extension.add("hello_world", hello_world);
```

Working with parameters and return values is just as easy:

```c
Php::Value my_plus(Php::Parameters &params)
{
    return params[0] + params[1];
}
```

The method call to export the above C++ function:

```c
extension.add("my_plus", my_plus, {
	Php::ByVal("a", Php::numericType),
	Php::ByVal("b", Php::numericType)
});
```

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

```c
Php::Value get_complex_array()
{
    Php::Value r;
    r["a"] = 123;
    r["b"] = 456;
    r["c"][0] = "nested value";
    r["c"][1] = "example";
    return r;
}
```

The C++ function above is equivalent to the following function in PHP:

```php
function get_complex_array()
{
    return array(
        "a" => 123,
        "b" => 456,
        "c" => array("nested_value","example")
    );
}
```

More information and more examples are available on the official website:
http://www.php-cpp.com.
