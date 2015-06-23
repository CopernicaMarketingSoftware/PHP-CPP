<?php
/**
 *  Test script to see if the ReturnObject extension works as it should
 *  
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Construct master object
 *  @var Master
 */
$master = new Master();

/**
 *  Construct derived child object
 *  @var Child
 */
$child1 = new Child();

/**
 *  Fetch the child object that is stored as member var in the master
 *  @var Child
 */
$child2 = $master->child();

/**
 *  Show output, expected is:
 *      this is the master
 *      this is the child
 *      this is the child
 */
echo(strval($master)."\n");
echo(strval($child1)."\n");
echo(strval($child2)."\n");

