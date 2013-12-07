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

// try-catch block to be able to handle the exception
try
{
    // the my_throw_exception function is implemented in C++. and
    // it is going to throw an exception
    my_throw_exception_function();
}
catch (Exception $exception)
{
    // the exception object is thrown by C++ code, and caught by PHP
    // code
    echo $exception->getMessage(). "\n";
    
}
