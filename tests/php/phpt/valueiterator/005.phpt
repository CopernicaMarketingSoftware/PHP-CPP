--TEST--
Iterate itarable oblect
--DESCRIPTION--
class impIterAggr1 implements IteratorAggregate
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php
require dirname(__FILE__) . '/../../include/valueiterator/1.php';

$iag1 = new impIterAggr1;

TestValueIterator\loopValue($iag1);

--EXPECT--
Array/Object contains 0 items
[cl0_float]=3.14
[cl0_str1]=public str1
~impIterAggr1