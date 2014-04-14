--TEST--
Test ini entries #2
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--INI--
ini2=ReValIni2
ini6=33
--FILEEOF--
<?php

var_export( ini_get_all ( 'extension_for_tests' ) );

--EXPECT--
array (
  'ini1' => 
  array (
    'global_value' => 'valIni1',
    'local_value' => 'valIni1',
    'access' => 7,
  ),
  'ini2' => 
  array (
    'global_value' => 'OrigValIni2',
    'local_value' => 'ReValIni2',
    'access' => 7,
  ),
  'ini3' => 
  array (
    'global_value' => 'OrigValIni3',
    'local_value' => 'valIni3',
    'access' => 4,
  ),
  'ini4' => 
  array (
    'global_value' => 'Off',
    'local_value' => 'On',
    'access' => 1,
  ),
  'ini5' => 
  array (
    'global_value' => 'Off',
    'local_value' => 'Off',
    'access' => 7,
  ),
  'ini6' => 
  array (
    'global_value' => '11',
    'local_value' => '33',
    'access' => 7,
  ),
  'ini7' => 
  array (
    'global_value' => '5',
    'local_value' => '74',
    'access' => 7,
  ),
  'ini8' => 
  array (
    'global_value' => '6.283185',
    'local_value' => '3.141593',
    'access' => 7,
  ),
  'ini9' => 
  array (
    'global_value' => '5.436564',
    'local_value' => '2.718282',
    'access' => 1,
  ),
)