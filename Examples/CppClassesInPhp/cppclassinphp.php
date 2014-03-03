<?php
/**
 *  cppclassinphp.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

class TestClass
{
    public $x = 1223;
    
    public function __construct()
    {
        echo("TestClass::__construct\n");
    }
    
}
 
//create a MyCustomClass object, which is an object of a C++ class
$object = new MyClass();

// run a function of the class
$obj = $object->myMethod("MyClass");

echo(get_class($obj)."\n");
//echo($obj->format("Y-m-d")."\n");

//echo($obj->x."\n");

return;

//$object->myMethod(2);

echo($object->property1."\n");
echo($object->property2."\n");
