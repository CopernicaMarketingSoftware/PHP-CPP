--TEST--
test superglobal _ENV
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--ENV--
return <<<END
ENVVAR1=ENVVAL1
HTTP_REFERER=http://spb.1gs.ru/
HTTP_USER_AGENT=Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:28.0) Gecko/20100101 Firefox/28.0
HTTP_COOKIE=usr=1cfa6e9905; pc=0; PHPSESSID=4d08j9phpe1c6; vmode=0
HTTP_HOST=phpcpp.com
END;
--FILEEOF--
<?php


TestVariables\test_env();
//var_export($_SERVER); echo PHP_EOL, PHP_EOL, PHP_EOL;

--EXPECTF--
HTTP_USER_AGENT => Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:28.0) Gecko/20100101 Firefox/28.0
ENVVAR1 => ENVVAL1
HTTP_REFERER => http://spb.1gs.ru/
REQUEST_METHOD => GET
HTTP_HOST => phpcpp.com
