--TEST--
Iterate object of stdClass
--DESCRIPTION--
Iterate object of stdClass
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php
require dirname(__FILE__) . '/../../include/valueiterator/1.php';

$arr  = array('qwe' => 'qweqweqweqw',5,'asd' => '«£¥§©®°±¶⅐⅒⅓⅘⅞Ⅻↆ❄❅❆⚑⚐⌛⌚〰»', 'zxccvx' => 'sdfsecvyh6bug6yfty',);
$obj  = (object)$arr;

TestValueIterator\loopValue($obj);

echo PHP_EOL;
--EXPECT--
Array/Object contains 0 items
[qwe]=qweqweqweqw
[0]=5
[asd]=«£¥§©®°±¶⅐⅒⅓⅘⅞Ⅻↆ❄❅❆⚑⚐⌛⌚〰»
[zxccvx]=sdfsecvyh6bug6yfty