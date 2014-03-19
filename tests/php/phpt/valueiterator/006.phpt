--TEST--
Iterate itarable oblect
--DESCRIPTION--
class impIterAggr1 implements IteratorAggregate
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php
require dirname(__FILE__) . '/../../include/valueiterator/1.php';

$iag2 = new impIterAggr2;

TestValueIterator\loopValue($iag2);

--EXPECT--
Array/Object contains 0 items
[key_0]=firstElement
[key_1]=secondElement
[key_2]=lastelEment
~impIterAggr2
~impIterator