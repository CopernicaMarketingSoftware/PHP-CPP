<?php

class XXX
{
    public function __toString()
    {
        return "MyClass";
    }
}

$myvar = "hoi";

class MyClass {

    public function __toString()
    {
        return "aksjdfhsdfkj";
    }
}

$g1 = 123;
$g2 = "abc";

$result = my_plus(new MyClass(), array(), new MyClass(), $myvar, "blabla", new XXX());

echo("myvar = $myvar\n");

echo("resultaat: $result\n");

echo("g1: $g1\n");
echo("g2: $g2\n");
echo("g3: $g3\n");


if (class_exists("my_class")) echo("Warempel, de class bestaat\n");

$x = new my_class();

echo("my_class::a = ".$x->a."\n");
echo("my_class::b = ".$x->b."\n");

unset($x);

echo("done\n");

//$x->my_method();


