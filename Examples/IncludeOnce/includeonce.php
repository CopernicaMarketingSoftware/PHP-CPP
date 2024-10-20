<?php
/**
 *  include_once.php
 * 
 *  An example file to show the working of a php function call in C++.
 */

/**
 *  Include the file (should include the file only once)
 */
my_include_once(__DIR__."/class.php");
my_include_once(__DIR__."/class.php");
my_include_once(__DIR__."/class.php");

/**
 *  Construct object
 */
$x = new SillyClass();
