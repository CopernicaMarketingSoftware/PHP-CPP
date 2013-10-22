<?php

//class XXX
//{
//    public function __toString()
//    {
//        return "MyClass";
//    }
//}
//
//$myvar = "hoi";
//
//class MyClass {
//
//    public function __toString()
//    {
//        return "aksjdfhsdfkj";
//    }
//}
//
//$g1 = 123;
//$g2 = "abc";
//
//$result = my_plus(new MyClass(), array(), new MyClass(), $myvar, "blabla", new XXX());
//
//echo("myvar = $myvar\n");
//
//echo("resultaat: $result\n");
//
//echo("g1: $g1\n");
//echo("g2: $g2\n");
//echo("g3: $g3\n");

function slowsort($input)
{
    $size = count($input);
    for ($i=0; $i<$size; $i++)
    {
        $left = $input[$i];
        for ($j=$i+1; $j<$size; $j++)
        {
            $right = $input[$j];
            
            if ($left < $right) continue;
            
            // swap values
            $input[$i] = $right;
            $input[$j] = $left;
            $left = $right;
        }
    }
    return $input;
}

//if (class_exists("my_class")) echo("Warempel, de class bestaat\n");
//
//class my_extended_class extends my_class
//{
//    public function myMethod($val)
//    {
//        echo($this->a."\n");
//        echo($this->b."\n");
//        echo("hoi\n");
//        
//        parent::myMethod($val);
//    }
//    
//}
//
//$x = new my_extended_class();
//$x->myMethod(123);

$x = new my_class();
$x->myMethod();


//echo(my_plus(1,2,3,4)."\n");

$array = array();
for ($i=0; $i<10000; $i++) $array[] = rand();

//$array = array(1,2,3);

//print_r($array);
//bubblesort($array);

//print_r($array);


//echo("my_class::a = ".$x->a."\n");
//echo("my_class::b = ".$x->b."\n");
//
//unset($x);
//
//echo("done\n");

//$x->my_method();


