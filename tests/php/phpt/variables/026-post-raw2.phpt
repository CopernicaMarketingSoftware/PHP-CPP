--TEST--
Post files
--DESCRIPTION--
Simulates the file upload method post
test post raw #2
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--POST_RAW--
Content-type: multipart/form-data, boundary=AaB03x

--AaB03x
content-disposition: form-data; name="flnm[]"; filename="file1.txt"
Content-Type: text/plain

abcdef123456789
--AaB03x
content-disposition: form-data; name="flnm[]"; filename="file2.txt"
Content-Type: text/plain

«£¥§©®°±¶Я⅓⅘⅞Ⅻↆ❄❅❆⚑⚐⌛⌚〰»
--AaB03x
--FILEEOF--
<?php


TestVariables\post_raw2();

--EXPECTF--
name1 : file1.txt
name2 : file2.txt
type1 : text/plain
type2 : text/plain
error1 : 0
error2 : 0
size1 : 15
size2 : 61
content1 : abcdef123456789
content2 : «£¥§©®°±¶Я⅓⅘⅞Ⅻↆ❄❅❆⚑⚐⌛⌚〰»