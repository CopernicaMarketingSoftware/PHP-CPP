--TEST--
calling-php-functions from userspace
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php



function some_function($param) {
    echo("userspace function called with parameters:\n");
    var_export(func_get_args ());
}
    
TestVariables\fnFromUserSpace();



--EXPECT--
fnFromUserSpace
userspace function called with parameters:
array (
  0 => 'param1',
)userspace function called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
)userspace function called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
)userspace function called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
)userspace function called with parameters:
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
)userspace function called with parameters:
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
)userspace function called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
)userspace function called with parameters:
array (
  0 => 'param1',
  1 => 'param2',
  2 => 'param3',
  3 => 'param4',
  4 => 'param5',
  5 => 'param6',
  6 => 'param7',
  7 => 'param8',
)userspace function called with parameters:
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
)userspace function called with parameters:
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
