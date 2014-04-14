--TEST--
test get and post
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--GET--
a=<b>test</b>&b=http://phpcpp.com&ar[elm1]=1234&ar[elm2]=0660&a=0234
--POST--
c=<p>string</p>&d=12345.7&e[]=e1&e[]=e2
--FILEEOF--
<?php


TestVariables\get_post();

--EXPECTF--
_GET[a] = 0234
_GET[b] = http://phpcpp.com
_GET[ar][elm1] = 1234
_GET[ar][elm2] = 0660
_POST[c]   = <p>string</p>
_POST[d]   = 12345.7

Notice: Array to string conversion in %s/024-get-post.php on line %d
_POST[e]   = Array
_POST[e][0]   = e1
_POST[e][1]   = e2