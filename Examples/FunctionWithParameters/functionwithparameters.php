<?php
/*
 *	functionwithparameters.php
 * 	@author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 * 	An example file to show the working of a function call with parameters, defined and undefined.
 */

/*
 *	Run a function with parameters. 
 */
echo 'A function which takes parameters, which are all undefined;' . 
	'\nmy_with_undefined_parameters_function("1st","2nd","3rd","4th")\n';
echo my_with_undefined_parameters_function("1st","2nd","3rd","4th") . '\n';

echo 'A function which takes parameters, which are all defined;' . 
	'\nmy_with_defined_parameters_function(21,42)\n';
echo my_with_defined_parameters_function(21,42) . '\n';

echo 'Accessing a non-existant parameters index will result in a segmentation fault.\n';
echo 'The segmentation fault will also occur when passing the wrong type of parameter.\n';
