--TEST--
Test ini entries #1
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--INI--
ini2=ReValIni2
--FILEEOF--
<?php

TestIniEntries\iniTest1();

--EXPECT--
ini_get(ini1) = valIni1
ini_get(ini2) = ReValIni2
ini_get(ini3) = valIni3
ini_get(ini4) = On
ini_get(ini5) = Off
ini_get(ini6) = 55
ini6val = 55