--TEST--
get_complex_array (test HashMember #4)
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

var_export(TestVariables\test_HashMember_4());


echo PHP_EOL;
--EXPECT--
array (
  0 => 
  array (
    'key1' => 
    array (
      'key2' => 
      array (
        'key3' => 'val',
      ),
    ),
  ),
  1 => 
  array (
    'str1' => 'example',
    'str2' => 'example',
  ),
  2 => 
  array (
    'str1' => 
    array (
      'str2' => 'val1-2',
      'str3' => 'val1-3',
    ),
  ),
)