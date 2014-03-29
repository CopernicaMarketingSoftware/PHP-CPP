--TEST--
cast objects to scalars
--DESCRIPTION--
http://www.phpcpp.com/documentation/special-features#casting-functions
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


function bool2str($b) {
    return $b ? "Yes" : "No";
}
$obj = new TestVariables\Obj2Scalar();
echo  "   long: ", (int)$obj, "\n string: ", (string)$obj, "\n double: ", (double)$obj, "\n   bool: ", bool2str((bool)$obj);


--EXPECT--
   long: 27032014
 string: Mount Meru, also called Sumeru (Sanskrit)
 double: 3.14159265359
   bool: Yes
