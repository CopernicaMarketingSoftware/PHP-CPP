--TEST--
Iterate itarable oblect
--DESCRIPTION--
class impIterator implements Iterator
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php
require dirname(__FILE__) . '/../../include/valueiterator/1.php';

$it   = new impIterator;

TestValueIterator\loopValue($it);

--EXPECT--
Array/Object contains 0 items
[key_0]=firstElement
[key_1]=secondElement
[key_2]=lastelEment
~impIterator