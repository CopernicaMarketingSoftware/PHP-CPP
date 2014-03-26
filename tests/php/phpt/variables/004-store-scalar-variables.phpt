--TEST--
Test variables defined in PHP-CPP
--DESCRIPTION--
in PHP-CPP:
Php::Value scalar_store(void) {
    
    Php::Value value1 = 1234;
    Php::Value value2 = "this is a string";
    Php::Value value3 = std::string("another string");
    Php::Value value4 = nullptr;
    Php::Value value5 = 123.45;
    Php::Value value6 = true;
    
    Php::Value r;
    r[0] = value1;
    r[1] = value2;
    r[2] = value3;
    r[3] = value4;
    r[4] = value5;
    r[5] = value6;
    
    r[6]  = 1234;
    r[7]  = "this is a string";
    r[8]  = std::string("another string");
    r[9]  = nullptr;        // NULL
    r[10] = Php::Value();   // NULL
    r[11] = 123.45;
    r[12] = false;
    
    return r;
}

--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


var_export(TestVariables\scalar_store());


//echo PHP_EOL;
--EXPECT--
array (
  0 => 1234,
  1 => 'this is a string',
  2 => 'another string',
  3 => NULL,
  4 => 123.45,
  5 => true,
  6 => 1234,
  7 => 'this is a string',
  8 => 'another string',
  9 => NULL,
  10 => NULL,
  11 => 123.45,
  12 => false,
)
