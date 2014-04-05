--TEST--
Test class with static property and class constant
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


var_dump( TestBaseClass\MyClass::CONSTANT1 );
var_dump( TestBaseClass\MyClass::EXP );
var_dump( TestBaseClass\MyClass::CONSTANT2 );
var_dump( TestBaseClass\MyClass::CONSTANT3 );

var_dump( TestBaseClass\MyClass::$StatProp1 );
var_dump( TestBaseClass\MyClass::$Exp );
var_dump( TestBaseClass\MyClass::$StatProp2 );
var_dump( TestBaseClass\MyClass::$StatProp3 );
TestBaseClass\MyClass::$StatProp2 = "otherval";
var_dump( TestBaseClass\MyClass::$StatProp2 );


//echo PHP_EOL;
--EXPECT--
string(11) "some string"
float(2.718281828459)
int(-2582341)
bool(true)
string(11) "some string"
float(2.718281828459)
int(-2582341)
bool(true)
string(8) "otherval"