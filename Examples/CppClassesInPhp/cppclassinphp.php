<?php
/**
 *  cppclassinphp.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */
 
//create a MyCustomClass object, which is an object of a C++ class
$object = new MyClass();

class ImplementedInterface implements MyInterface
{
}

$object2 = new ImplementedInterface();

return;

// run a function of the class
$object->myMethod(1);

echo($object->property1."\n");
echo($object->property2."\n");
