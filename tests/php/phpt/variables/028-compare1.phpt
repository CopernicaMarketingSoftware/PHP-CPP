--TEST--
Test bool Value::operator==(const Value &value) const
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

TestVariables\test_compare1();

--EXPECT--
true:
1
1
1
1
1
1
1
1
1
1
1
1
false:
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
Compare array:
0
0
0
1
0
Compare NULL:
0
0
0
1
1
1