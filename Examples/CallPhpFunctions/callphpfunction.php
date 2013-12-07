<?php
/**
 *  callphpfunction.php
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of a php function call in C++.
 */

class MyClass
{
    function method($a,$b,$c)
    {
        return $a+$b+$c;
    }
}

function myFunction($a,$b,$c)
{
    return $a+$b+$c;
}

/**
 *  Call a C++ function with a callable PHP function as its parameter.
 *  The PHP function is then executed from the C++ code.
 *  The PHP function is this case, adds three numbers.
 */
echo(call_php_function(function($a, $b, $c){
                                return $a + $b + $c;
                            })."\n");
                            
echo(call_php_function("myFunction")."\n");
                            
echo(call_php_function(array(new MyClass(), 'method'))."\n");
