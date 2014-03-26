--TEST--
Test Php::Value object #2
--DESCRIPTION--
 create empty object of type stdClass
 object properties can be accessed with square brackets
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


//date_default_timezone_set('Europe/Amsterdam');
var_export(TestVariables\value_object2());


--EXPECT--
2014-03-27 00:37:15
2016-03-31 15:48:00
7200
DateTime::__set_state(array(
   'date' => '2014-03-27 00:37:15',
   'timezone_type' => 3,
   'timezone' => 'Europe/Amsterdam',
))