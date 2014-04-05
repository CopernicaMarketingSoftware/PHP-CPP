--TEST--
get_complex_array (test HashMember #3)
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

/*
$r = array();
$r["key1"] = array();
$r["key1"]["key2"] = "val1-2";
$r["key1"]["key3"] = "val1-3";
var_export($r);echo "\n";
*/

var_export(TestVariables\test_HashMember_3());


echo PHP_EOL;
--EXPECT--
array (
  'key1' => 
  array (
    'key2' => 'val1-2',
    'key3' => 'val1-3',
  ),
)