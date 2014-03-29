--TEST--
Test Php::Value object #1
--DESCRIPTION--
 create empty object of type stdClass
 object properties can be accessed with square brackets
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


$object = TestVariables\value_object1();

//$object = (object)array("property1" => "value1", "property2" => "value2");

var_export($object);



--EXPECT--
stdClass::__set_state(array(
   'property1' => 'value1',
   'property2' => 'value2',
))