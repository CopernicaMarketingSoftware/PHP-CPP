--TEST--
test constants functions(constant, define, defined)
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


TestVariables\test_constants();

--EXPECT--
hello
Yes
No
