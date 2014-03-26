--TEST--
get_complex_array
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


# check Callable: http://php.net/manual/en/function.is-callable.php
function  ret5() {return 5;}
class someClass {
  function someMethod() {return 5;}
}
class CallableClass {
    public function __invoke(){}
}

$anObject = new someClass();
$methodVariable = array($anObject, 'someMethod');


# check array
$arr = array(
    'Null'            =>   NULL,
    'Numeric'         =>   2014,
    'Float'           =>   3.14,
    'Bool'            =>   true,
    'Array'           =>   array(5,'a' => 33, 'str'),
    'Object'          =>   new stdClass(),
    'String'          =>   'String',
    'Resource'        =>   7,
    'Constant'        =>   5,
    'ConstantArray'   =>   11,
    'Callable1'        =>  'ret5',
    'Callable2'        =>  $methodVariable,
    'Callable3'        =>  function () {return 5;},
    'Callable4'        =>  new CallableClass()
);

TestVariables\value_types($arr);

//To check uncomment the following lines:
/*
echo "\n\nCallable1:";
var_export(is_callable($arr['Callable1']));
echo PHP_EOL,'Callable2:';
var_export(is_callable($arr['Callable2']));
echo PHP_EOL,'Callable3:';
var_export(is_callable($arr['Callable3']));
echo PHP_EOL,'Callable4:';
var_export(is_callable($arr['Callable4']));
echo PHP_EOL,'No Callable:';
var_export(is_callable(new stdClass));
*/


echo PHP_EOL;
--EXPECT--
Null: Yes
Numeric: Yes
Float: Yes
Bool: Yes
Array: Yes
Object: Yes
String: Yes
Resource: No
Constant: No
ConstantArray: No
Callable1: Yes
Callable2: Yes
Callable3: Yes
Callable4: Yes