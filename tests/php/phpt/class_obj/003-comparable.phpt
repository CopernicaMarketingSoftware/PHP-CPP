--TEST--
Test constructor & destructor
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

// initialize a couple of objects
$object1 = new TestBaseClass\Comparable();
$object2 = new TestBaseClass\Comparable();
$object3 = new TestBaseClass\Comparable();

// compare the objects
if ($object1 < $object2)
{
    echo("$object1 is smaller than $object2\n");
}
else
{
    echo("$object1 is bigger than $object2\n");
}

if ($object1 == $object3)
{
    echo("$object1 is equal to $object3\n");
}
else
{
    echo("$object1 is not equal to $object3\n");
}

if ($object1 != $object2)
{
    echo("$object1 is not equal to $object2\n");
}
else
{
    echo("$object1 is equal to $object2\n");
}


//echo PHP_EOL;
--EXPECT--
Obj#1(2) is bigger than Obj#2(1)
Obj#1(2) is equal to Obj#3(2)
Obj#1(2) is not equal to Obj#2(1)