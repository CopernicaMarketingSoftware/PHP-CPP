<?php

/**
  *  For debugging tests
  *  
  */







TestIniEntries\iniTest1();
/*
exit;
ini_restore ( 'ini1' );
ini_restore ( 'ini2' );
ini_restore ( 'ini3' );
TestIniEntries\iniTest1();
*/
//exit;
echo "\x1b[1;31m";
(new ReflectionExtension('extension_for_tests') )->info();
echo "\x1b[0m";
echo "\x1b[0;34m";
var_export( ini_get_all ( 'extension_for_tests' ) );
echo "\x1b[0m", PHP_EOL;
exit;

TestIniEntries\iniTest1();

//ini_set("extension_for_tests.some_string3", 'RFVBGT') ;
echo PHP_EOL;
echo "extension_for_tests.some_string ={{" ,  ini_get("extension_for_tests.some_string") ,  "}}", PHP_EOL;
echo "extension_for_tests.some_string2 ={{" , ini_get("extension_for_tests.some_string2") , "}}", PHP_EOL;
echo "extension_for_tests.some_string3 ={{" , ini_get("extension_for_tests.some_string3") , "}}", PHP_EOL;
echo "extension_for_tests.some_string3 ={{" , ini_get("extension_for_tests.some_bool")    , "}}", PHP_EOL;


echo "\x1b[1;31m";
(new ReflectionExtension('extension_for_tests') )->info();
echo "\x1b[0m";

echo "\x1b[0;34m";
var_export( ini_get_all ( 'extension_for_tests' ) );
echo "\x1b[0m", PHP_EOL;

ini_set("extension_for_tests.some_string3", 'RFVBGT') ;
echo "extension_for_tests.some_string3 ={{" , ini_get("extension_for_tests.some_string3") , "}}", PHP_EOL;
ini_restore ( 'extension_for_tests.some_string3' );
echo "extension_for_tests.some_string3 ={{" , ini_get("extension_for_tests.some_string3") , "}}", PHP_EOL;


echo "extension_for_tests.some_bool :";
var_dump(ini_get("extension_for_tests.some_bool"));
echo PHP_EOL;

ini_set("extension_for_tests.some_bool", 'off');

echo "extension_for_tests.some_bool :";
var_dump(ini_get("extension_for_tests.some_bool"));
echo PHP_EOL;

//var_export(php_sapi_name());
