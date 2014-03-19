--TEST--
Iterate object
--DESCRIPTION--
Iterate regular php object
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php
require dirname(__FILE__) . '/../../include/valueiterator/1.php';

$smpl = new SimpleClass;

TestValueIterator\loopValue($smpl);

--EXPECT--
Array/Object contains 0 items
[cl0_float]=3.14
[cl0_str1]=public str1