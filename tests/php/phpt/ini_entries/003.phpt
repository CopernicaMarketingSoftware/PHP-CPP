--TEST--
Test ini entries #3
--DESCRIPTION--
Test to check palce action of ini entries
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php

echo ini_get('ini2'), PHP_EOL;
echo ini_get('ini3'), PHP_EOL;
ini_set('ini2', 'newIni2');
ini_set('ini3', 'newIni3');
echo ini_get('ini2'), PHP_EOL;
echo ini_get('ini3'), PHP_EOL;
ini_restore('ini2');
ini_restore('ini3');
echo ini_get('ini2'), PHP_EOL;
echo ini_get('ini3');

--EXPECT--
valIni2
valIni3
newIni2
valIni3
valIni2
valIni3