<?php
/*
 *  globals.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a void function call.
 */

// we first need to set a number of globals
$b = 10;
$d = function($a,$b,$c) {
    return $a+$b+$c;
};

// fun global var
$e = array(
    function($a) {
        return $a;
    }
);


// call the C++ function that will do something
$d = process_globals();

// the global variable $a should not have the value 1
echo("a = $a\n");

// the variable $b should not be 11
echo("b = $b\n");

// $c should be an array with value 200
echo("c['member'] = ".$c['member']."\n");

// $d is overwritten and now is 6
echo("d = $d\n");

// e should be replaced by a string
echo("e = $e\n");