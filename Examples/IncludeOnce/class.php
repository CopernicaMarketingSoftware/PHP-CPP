<?php
/**
 *  Class.php
 *
 *  Empty class, we just try to double-include this file to run into a
 *  "class already defined" error
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2024 Copernica BV
 */

/**
 *  Include ourselves (should do nothing)
 */
include_once(__FILE__);
my_include_once(__FILE__);

/**
 *  Class definition
 */
class SillyClass {}
