--TEST--
Test class with static function
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

// this will call PublicClass::staticMethod()
TestBaseClass\ClassWithStatic::static1();

// this will call PrivateClass::staticMethod()
TestBaseClass\ClassWithStatic::static2();

// this will call regularFunction()
TestBaseClass\ClassWithStatic::static3();

// this will also call PrivateClass::staticMethod()
TestBaseClass\staticFun1();


//echo PHP_EOL;
--EXPECT--
testStaticPubClass::staticMethod()
testStatic regular function
testStaticPrivClass::staticMethod()
testStaticPrivClass::staticMethod()