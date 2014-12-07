--TEST--
Test Php::Value casting operators (64bit OS only)
--DESCRIPTION--
The Php::Value class has casting operators to cast the object to almost every thinkable native type.

native_value_casting - repeats the behavior of c++ functions TestVariables\value_casting
the output of each of these functions should be the same

Skip if the current OS is not 64-bit architecture.

--SKIPIF--
<?php if (!extension_loaded("extension_for_tests") || 'x86_64' != php_uname('m') ) print "skip"; ?>
--FILEEOF--
<?php


function bool2str($b) {
    return $b ? "Yes" : "No";
}
function native_value_casting($value) {
    //echo  "   long:", (int)$value, "\n string:", (string)$value, "\n double:", (double)$value, "\n   bool:", bool2str((bool)$value), PHP_EOL;
    echo  "   long:", (int)$value, "\n string:", (string)$value, "\n   bool:", bool2str((bool)$value), PHP_EOL;
}


function out($v) {
    echo 'Test ';
    var_export($v);
    echo ':', PHP_EOL;
    TestVariables\value_casting($v);
    //native_value_casting($v);
    echo PHP_EOL;
}

out(5555555555555555);
out("9223372036854775807");
out(9223372036854775807);
out(5);
out(-99999999);
out("18");
out("3.1415926535898");
out(3.14159265358979);
out(25/7);
out(" this is a string ");
out(" 2-nd string ");
out("false");
out(false);
out("true");
out(true);
out(0);
out("0");
out(0123); // octal number (equivalent to 83 decimal)
out(0x1A); // hexadecimal number (equivalent to 26 decimal)




//echo PHP_EOL;
--EXPECT--
Test 5555555555555555:
   long:5555555555555555
 string:5555555555555555
   bool:Yes

Test '9223372036854775807':
   long:9223372036854775807
 string:9223372036854775807
   bool:Yes

Test 9223372036854775807:
   long:9223372036854775807
 string:9223372036854775807
   bool:Yes

Test 5:
   long:5
 string:5
   bool:Yes

Test -99999999:
   long:-99999999
 string:-99999999
   bool:Yes

Test '18':
   long:18
 string:18
   bool:Yes

Test '3.1415926535898':
   long:3
 string:3.1415926535898
   bool:Yes

Test 3.14159265358979:
   long:3
 string:3.1415926535898
   bool:Yes

Test 3.5714285714285716:
   long:3
 string:3.5714285714286
   bool:Yes

Test ' this is a string ':
   long:0
 string: this is a string 
   bool:Yes

Test ' 2-nd string ':
   long:2
 string: 2-nd string 
   bool:Yes

Test 'false':
   long:0
 string:false
   bool:Yes

Test false:
   long:0
 string:
   bool:No

Test 'true':
   long:0
 string:true
   bool:Yes

Test true:
   long:1
 string:1
   bool:Yes

Test 0:
   long:0
 string:0
   bool:No

Test '0':
   long:0
 string:0
   bool:No

Test 83:
   long:83
 string:83
   bool:Yes

Test 26:
   long:26
 string:26
   bool:Yes
