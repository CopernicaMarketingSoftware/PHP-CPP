The namespace in PHP is not defined no internal structure.
It is only a part of the name defined in it a class or function.
When we write
```php
<?php namespace TestNamespace;
class Foo {}
?>
```
the parser simply appends 'TestNamespace\' as a prefix to the name of the Foo class.
This is evident if you look in the interior of the Zend Engine.
The only place, where we are confronted with the namespace in the Zend Engine -- this structure `struct _zend_compiler_globals {}` в /path/to/php-src/Zend/zend_globals.h
Mention namespace also occurs in /path/to/php-src/Zend/zend_compile.h in functions such as
int zend_do_begin_function_call(znode *function_name, zend_bool check_namespace TSRMLS_DC);
That just confirms what was said.
This may not be obvious, if not to look at the insides of Zend.

So the following code in PHP
```php
<?php
class TestNamespace\Foo {}
?>
```
will cause error analyzer `Parse error: syntax error, unexpected '\' `
When we coding extension for PHP in C or C++, we no need to inform the parser.
We can just append the prefix `TestNamespace\` to a names of defined classes or functions, and it will work.

example:
PHP-CPP:
```c
        ...
        extension.add(
            "NamespaceExample\\TestClass", 
            Php::Class<MyTestClass>({
                Php::Public("myMethod", Php::Method<MyTestClass>(&MyTestClass::myMethod)),
            })
        );

        extension.add("NamespaceExample\\TestFunc", MyTestFunc);
        ...
```
PHP:
```php
<?php namespace NamespaceExample;
var_dump(new TestClass());

echo TestFunc();

$refl = new \ReflectionClass('NamespaceExample\TestClass');
echo "\nNamespaceName: ", $refl->getNamespaceName();

echo "\nTestClass inNamespace: " ;
var_export($refl->inNamespace());
```
output:
```
object(NamespaceExample\TestClass)#1 (0) {
}
I'm is TestFunc
NamespaceName: NamespaceExample
TestClass inNamespace: true
```
