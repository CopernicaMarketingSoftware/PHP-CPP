--TEST--
Test Php::Value casting operators (object to string)
--DESCRIPTION--

Converting a php object to a string


--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


function native_value_casting($value) {
    echo  (string)$value, PHP_EOL;
}
class TestClass
{
    public function __toString() {return "I am TestClass";}
}

TestVariables\value_cast2str(new TestClass());
//native_value_casting(new TestClass());

TestVariables\value_cast2str("some string");



--EXPECT--
I am TestClass
some string