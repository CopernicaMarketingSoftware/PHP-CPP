<?php
/*
 *  functionwithparameters.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a function call with parameters, defined and undefined.
 */

/*
 *  Test class.
 */
class MyPhpClass {

    public $aMemberVar = "aMemberVar";

    public function __toString()
    {
        return "myPhpClass.__toString()";
    }
    
    public function getMemberVar()
    {
        return $aMemberVar;
    }
}

/*
 *  Run a function with parameters. 
 */
 

/* 
 * A function which takes parameters, which are all undefined;
 * my_with_undefined_parameters_function('1st','2nd','3rd','4th')
 */
echo    my_with_undefined_parameters_function('1st','2nd','3rd','4th') . "\n\n\n";

/*
 * A function which takes parameters, which are all defined;
 * my_with_defined_parameters_function(21,42)
 */
        
echo    my_with_defined_parameters_function(21,42) . "\n\n\n";

/* 
 * A function which takes a reference of a parameter
 * my_with_defined_parameters_reference_function(referenceVar)
 */
    
$referenceVar = "I am unchanged.";
echo "The value of referenceVar: " . $referenceVar. "\n";

echo my_with_defined_parameters_reference_function($referenceVar) . "\n";

echo "New value of referenceVar: " . $referenceVar ."\n\n\n";

/*
 * A function which takes an array as a parameter
 * my_with_defined_array_parameters_function($myArray)
 */
$myArray = array("a", "b", "c", "d");
echo my_with_defined_array_parameters_function($myArray) . "\n\n\n";

/*
 * A function which takes an object as a parameter
 * my_with_defined_object_parameter_function(myPhpClass)
 */
        
$myPhpClass = new MyPhpClass;
echo    my_with_defined_object_parameters_function($myPhpClass);

/*
 * Accessing a non-existant parameters index will result in a segmentation fault.
 * The segmentation fault will also occur when passing the wrong type of parameter.
 */
