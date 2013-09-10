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

$result = my_plus(new MyClass(), array(), new MyClass(), $myvar, "blabla", new XXX());

echo("myvar = $myvar\n");

echo("resultaat: $result\n");

print_r($result);

