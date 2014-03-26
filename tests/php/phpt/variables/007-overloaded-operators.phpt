--TEST--
Test Php::Value overloaded operators 
--DESCRIPTION--

Many different operators are overloaded too so that you can use a Php::Value object directly in arithmetric operations,
compare it with other variables, or send it to an output stream. 


--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


TestVariables\overloaded_op(5);
TestVariables\overloaded_op(3.14);
TestVariables\overloaded_op(12);
TestVariables\overloaded_op(120);
TestVariables\overloaded_op(-1000);
TestVariables\overloaded_op("some string");



--EXPECT--
15
7
1851.75
0.121507
13.14
5
1622.13
0.10644
value == 12
22
14
2715.9
0.17821
value > 100
130
122
16048.5
1.05306
-990
-998
-122216
-8.01944
value == 'some string'
10
2
1234.5
0.0810045