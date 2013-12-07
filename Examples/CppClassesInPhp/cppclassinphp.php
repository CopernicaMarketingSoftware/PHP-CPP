<?php
/**
 *  cppclassinphp.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

//create a MyCustomClass object, which is an object of a C++ class
$MyCustomClass = new MyClass();

// run a function of the class
$MyCustomClass->myMethod(1);
