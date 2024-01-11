<?php
/**
 *  extismcpp.php
 *  @author David Buzz <davidbuzz@gmail.com>
 * 
 *  inspired by some of the PHP-CPP examples as good reference material
 */


 
//create a ExtismClass object, which is an object of a C++ class
$extismInstance = new ExtismClass(); // when called with no params , it inits with -1


//$another_extism_Instance = $extismInstance->Factory(1);  // object with the default value -1 created above, instantiates a new one defaulting to 1.


$result = $extismInstance->LoadWASM("./code-functions.wasm");
echo "------------------------------\n";
echo("LoadWASM count vowels result: ".$result."\n");
echo "------------------------------\n";

