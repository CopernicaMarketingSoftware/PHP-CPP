<?php
/*
 *	functionwithparameters.php
 * 	@author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 * 	An example file to show the working of a function call with parameters, defined and undefined.
 */

/*
 * 	Test class.
 */
class MyPhpClass {

	public $aMemberVar = "aMemberVar";

    public function __toString()
    {
        return "MyPhpClass.__toString()";
    }
}

/*
 *	Run a function with parameters. 
 */
echo "A function which takes parameters, which are all undefined;" . 
	"\n my_with_undefined_parameters_function('1st','2nd','3rd','4th')\n";
echo my_with_undefined_parameters_function('1st','2nd','3rd','4th') . "\n\n\n";

echo "A function which takes parameters, which are all defined; " . 
		"\nmy_with_defined_parameters_function(21,42)\n";
echo 	my_with_defined_parameters_function(21,42) . "\n\n\n";

echo "A function which takes a reference of a parameter;" .
		"\nmy_with_defined_parameters_reference_function(referenceVar)\n";
	
$referenceVar = "I am unchanged.";
echo "The value of referenceVar: " . $referenceVar. "\n";

echo my_with_defined_parameters_reference_function($referenceVar) . "\n";

echo "New value of referenceVar: " . $referenceVar ."\n\n\n";

$myPhpClass = new MyPhpClass;
echo "A function which takes an object as a parameter;" . 
		"\nmy_with_defined_object_parameter_function(myPhpClass)";
echo my_with_defined_object_parameters_function($myPhpClass) . "\n\n\n";

echo "Accessing a non-existant parameters index will result in a segmentation fault.\n";
echo "The segmentation fault will also occur when passing the wrong type of parameter.\n";
