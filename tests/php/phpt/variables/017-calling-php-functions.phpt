--TEST--
calling-php-Callback-functions (method of class)
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


class someClass {
    function someMethod() {
        echo("Callback called with parameters:\n");
        var_export(func_get_args ());
    }
}

$anObject = new someClass();
$methodVariable = array($anObject, 'someMethod');
TestVariables\fnCallback($methodVariable);


--EXPECT--
call callback
Callback called with parameters:
array (
  0 => 'param1',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 
  array (
    0 => 'param5',
    1 => 3.14159265358979,
    2 => 28032014,
    'key' => 'value',
  ),
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 
  array (
    0 => 'param5',
    1 => 3.14159265358979,
    2 => 28032014,
    'key' => 'value',
  ),
  5 => 
  DateTime::__set_state(array(
     'date' => '2014-03-28 19:42:15',
     'timezone_type' => 3,
     'timezone' => 'Asia/Yekaterinburg',
  )),
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
  7 => 'param8',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
  7 => 'param8',
  8 => 'param9',
)Callback called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
  7 => 'param8',
  8 => 'param9',
  9 => 'param10',
)
