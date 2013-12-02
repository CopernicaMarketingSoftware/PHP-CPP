<?php
/**
 *	callphpfunction.php
 * 	@author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 * 	An example file to show the working of a php function call in C++.
 */

/*
 *	Run the function with the given name. As you can see, the given name 
 * 	can be different from the actual function name.
 */
$result = call_php_function(function($a, $b, $c){
							return $a + $b + $c;
							});
echo $result;
