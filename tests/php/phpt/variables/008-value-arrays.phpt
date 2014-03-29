--TEST--
Test Php::Value arrays 
--DESCRIPTION--
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


$r = TestVariables\value_arrays();

    // To check:
/*
    $array;
    $array[0] = "apple";
    $array[1] = "banana";
    $array[2] = "tomato";

    // an initializer list can be used to create a filled array 
    $filled = ["a", "b", "c", "d"];

    // create an associative array
    $assoc["apple"] = "green";
    $assoc["banana"] = "yellow";
    $assoc["tomato"] = "green";

    // the variables in an array do not all have to be of the same type
    $assoc2["x"] = "info@example.com";
    $assoc2["y"] = NULL;
    $assoc2["z"] = 123;

    // nested arrays are possible too
    $assoc3["x"] = "info@example.com";
    $assoc3["y"] = NULL;
    $assoc3["z"][0] = "a";
    $assoc3["z"][1] = "b";
    $assoc3["z"][2] = "c";

    $r = [];
    $r["array"]  = $array;
    $r["filled"] = $filled;
    $r["assoc"]  = $assoc;
    $r["assoc2"] = $assoc2;
    $r["assoc3"] = $assoc3;
*/
    
var_export($r);



--EXPECT--
array (
  'array' => 
  array (
    0 => 'apple',
    1 => 'banana',
    2 => 'tomato',
  ),
  'filled' => 
  array (
    0 => 'a',
    1 => 'b',
    2 => 'c',
    3 => 'd',
  ),
  'assoc' => 
  array (
    'apple' => 'green',
    'banana' => 'yellow',
    'tomato' => 'green',
  ),
  'assoc2' => 
  array (
    'x' => 'info@example.com',
    'y' => NULL,
    'z' => 123,
  ),
  'assoc3' => 
  array (
    'x' => 'info@example.com',
    'y' => NULL,
    'z' => 
    array (
      0 => 'a',
      1 => 'b',
      2 => 'c',
    ),
  ),
)