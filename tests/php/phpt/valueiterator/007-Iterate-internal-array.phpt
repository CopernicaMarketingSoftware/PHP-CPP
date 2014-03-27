--TEST--
Iterate internal array
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

TestValueIterator\loopArray();

--EXPECT--
Array/Object contains 4 items
[0]=val0
[1]=val1
[third]=val3
[fourth]=val3
