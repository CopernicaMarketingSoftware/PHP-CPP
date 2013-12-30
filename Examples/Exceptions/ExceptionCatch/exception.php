<?php
/**
 *  exception.cpp
 * 
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 * 
 *  This example shows the working of a C++ function that throws an 
 *  exception, and that exception is then caught by PHP code.
 *  
 */

// call the second C++ function that accepts a callback
my_catch_exception_function(function($a, $b, $c) {

    // throw an exception from PHP - the C++ code will catch this exception
    throw new Exception("Example exception");
});
