--TEST--
Test constructor & destructor
--SKIPIF--
<?php if (!extension_loaded("extention_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

new TestBaseClass\MyClass();


echo PHP_EOL;
--EXPECT--
MyCustomClass::MyCustomClass()
MyCustomClass::~MyCustomClass