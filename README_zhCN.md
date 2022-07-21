PHP-CPP
=======

[![Build Status](https://travis-ci.org/CopernicaMarketingSoftware/PHP-CPP.svg?branch=master)](https://travis-ci.org/CopernicaMarketingSoftware/PHP-CPP)

PHP-CPP 库是一个 C++ 库为了开发 PHP 扩展，它提供了一个良好的文档集合和容易使用的类库以及扩展去构建本地 PHP 扩展，
完整的文档可以在 http://www.php-cpp.com 找到

**注意：仅仅支持 PHP 7**
这个库已经更新，可以用于 PHP 7.0 及以上版本，如果你想创造旧 PHP 版本的扩展，使用 [PHP-CPP-LEGACY](https://github.com/CopernicaMarketingSoftware/PHP-CPP-LEGACY)
库代替，PHP-CPP 和 PHP-CPP-LEGACY 库有几乎相同的api，这样您就可以轻松地为 PHP 5.* 移植扩展到 PHP 7，然后反过来。

关于
===
PHP-CPP 是由 Copernica (www.copernica.com) 创建和维护的,我们写
我们的代码主要使用 PHP 和 C++，需要一种有效的方法将这两种语言结合起来，这就是 PHP-CPP 的用武之地，
您是否欣赏我们的工作，是否正在寻找其他高质量的解决方案?

然后看看我们的其他解决方案:

* PHP-JS (www.php-js.com)
* Copernica Marketing Suite (www.copernica.com)
* MailerQ MTA (www.mailerq.com)
* Responsive Email web service (www.responsiveemail.com)

PHP-CPP的强大功能
====================

与常规的 PHP 扩展不同，后者很难实现，需要深入的了解 Zend 引擎和指针操作使用 PHP-CPP 构建的扩展
并不难开发。事实上，你唯一需要做的就是写一个函数 PHP-CPP 库使用了 C++ 11 提供的所有功能来转换参数并返回
函数和PHP之间的值:

```c
Php::Value hello_world()
{
    return "hello world!";
}
```

上面的函数是一个原生 C++ 函数。通过 PHP-CPP，您可以导出此函数到 PHP，只需要调用一个 C++ 方法:

```c
extension.add("hello_world", hello_world);
```

使用参数和返回值同样简单:

```c
Php::Value my_plus(Php::Parameters &params)
{
    return params[0] + params[1];
}
```

导出上述 C++ 函数的方法调用:

```c
extension.add<my_plus>("my_plus", {
    Php::ByVal("a", Php::numericType),
    Php::ByVal("b", Php::numericType)
});
```

PHP-CPP 库确保变量 从 PHP (其内部是复杂的C结构)，自动转换为传递给函数的整数，以及 “my_plus” 函数的返回值
也转换回PHP变量。

原生 C/C++ 类型和 PHP 变量之间的类型转换由 PHP-CPP 处理，使用 C++ 11 语言的特性。函数是否接受字符串并不重要，
整数、布尔值或其他本机参数: PHP-CPP 负责转换。PHP-CPP 还将函数的返回值转换为 PHP。

PHP-CPP 也可以处理更复杂的结构。如果你想回来的话一个嵌套的关联数组，你也可以这样做:

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

上面的 C++ 函数等价于 PHP 中的以下函数:

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

更多的信息和例子可以在官方网站上找到: http://www.php-cpp.com.