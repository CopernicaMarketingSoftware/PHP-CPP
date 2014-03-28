--TEST--
Test class with static property and class constant
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

var_dump( TestBaseClass\MyClass::CONSTANT1 );
var_dump( TestBaseClass\MyClass::EXP );
var_dump( TestBaseClass\MyClass::CONSTANT2 );
var_dump( TestBaseClass\MyClass::CONSTANT1 );


//echo PHP_EOL;
--EXPECT--
bool(true)
float(2.718281828459)
int(-2582341)
bool(true)