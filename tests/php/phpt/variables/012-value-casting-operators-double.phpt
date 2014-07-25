--TEST--
Test Php::Value casting operators (double)
--DESCRIPTION--
The Php::Value class has casting operators to cast the object to almost every thinkable native type.

native_value_casting - repeats the behavior of c++ functions TestVariables\value_casting
the output of each of these functions should be the same
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php



function out($v) {
    echo 'Test ';
    var_export($v);
    echo ':', PHP_EOL;
    TestVariables\value_cast2double($v);
    //echo  (double)$v, PHP_EOL;;
    //echo PHP_EOL;
}

out(5555555555555);
out("999999999999999");
out(99999999999999);
out(5);
out(-99999999);
out("18");
out("3.14159265358979");
out(3.14159265358979);
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


--EXPECT--
Test 5555555555555:
5555555555555
Test '999999999999999':
999999999999999
Test 99999999999999:
99999999999999
Test 5:
5
Test -99999999:
-99999999
Test '18':
18
Test '3.14159265358979':
3.14159265358979
Test 3.14159265358979:
3.14159265358979
Test ' this is a string ':
0
Test ' 2-nd string ':
2
Test 'false':
0
Test false:
0
Test 'true':
0
Test true:
1
Test 0:
0
Test '0':
0
Test 83:
83
Test 26:
26
