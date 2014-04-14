--TEST--
test post raw #1
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--POST_RAW--
Content-type: multipart/form-data, boundary=AaB03x

--AaB03x
content-disposition: form-data; name="username"

valmat
--AaB03x
content-disposition: form-data; name="text"

Hellow World!
--AaB03x
--FILEEOF--
<?php


TestVariables\post_raw1();

//foreach($_POST as $k => $v) {echo "\n$k => $v";}

--EXPECTF--
username => valmat
text => Hellow World!