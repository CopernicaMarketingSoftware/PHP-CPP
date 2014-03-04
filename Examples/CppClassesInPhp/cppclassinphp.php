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
$object1 = new MyClass();


$object2 = $object1->myMethod(1);
$object2->myMethod(1);

echo("unset\n");

unset($object1);

echo("got here\n");

return;

//echo("prop x: ".$object1->x."\n");

$object1->x = 10;
$object1->y = 20;

echo("prop x: ".$object1->x."\n");
echo("prop y: ".$object1->y."\n");

$object2 = clone $object1;

echo("prop x: ".$object2->x."\n");
echo("prop y: ".$object2->y."\n");


return;

// run a function of the class
$obj = $object->myMethod("MyClass");

echo(get_class($obj)."\n");
//echo($obj->format("Y-m-d")."\n");

//echo($obj->x."\n");

return;

//$object->myMethod(2);

echo($object->property1."\n");
echo($object->property2."\n");
