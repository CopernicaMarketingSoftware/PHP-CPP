<?php
/*
 *	exception.cpp
 * 	@author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 * 	An example file to show the working of a function which throws an exception.
 * 	
 * 	Exceptions haven't been implemented yet. 
 * 	Therefore this example is not yet a working one.
 */

/*
 *	Run a function which throws an exception.
 */

try
{
	echo "Function which throws an exception; my_throw_exception_function()\n";
	echo my_throw_exception_function() . "\n";
}
catch (Exception $exception)
{
	echo $exception->getMessage();
	
}
	echo my_throw_exception_function() . "\n";
