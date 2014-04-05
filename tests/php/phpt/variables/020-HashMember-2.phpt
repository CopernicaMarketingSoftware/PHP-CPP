--TEST--
get_complex_array (test HashMember #2)
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

/*
$r = array();
$r["k1"]["k3"] = "v1";
$r["k1"]["k2"]["k4"] = "v2";
$r["k5"][1] = "v3";
$r[2]["k6"][1] = "v4";
$r[3][4][1] = "v5";
$r[3][4][2][5][7][11] = "v5";
$r[3][4][2][5][7]["k"] = "v5";
$r["c"][0] = "nested value";
$r["c"][1] = NULL;
$r["c"][2] = array();
$r["c"][3] = "example";
var_export($r);echo "\n";
*/

var_export(TestVariables\test_HashMember_2());


echo PHP_EOL;
--EXPECT--
array (
  'k1' => 
  array (
    'k3' => 'v1',
    'k2' => 
    array (
      'k4' => 'v2',
    ),
  ),
  'k5' => 
  array (
    1 => 'v3',
  ),
  2 => 
  array (
    'k6' => 
    array (
      1 => 'v4',
    ),
  ),
  3 => 
  array (
    4 => 
    array (
      1 => 'v5',
      2 => 
      array (
        5 => 
        array (
          7 => 
          array (
            11 => 'v5',
            'k' => 'v5',
          ),
        ),
      ),
    ),
  ),
  'c' => 
  array (
    0 => 'nested value',
    1 => NULL,
    2 => 
    array (
    ),
    3 => 'example',
  ),
)