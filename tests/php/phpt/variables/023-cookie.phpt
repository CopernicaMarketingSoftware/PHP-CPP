--TEST--
test cookie
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--COOKIE--
peace=toTheWorld;freedom=toThePeople
--FILEEOF--
<?php


TestVariables\getCookie();

--EXPECT--
_COOKIE[peace]   = toTheWorld
_COOKIE[freedom] = toThePeople
_COOKIE[empty]   =