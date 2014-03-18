<?php

require '../../../php/class.screennav.php';
require '../files/set_get.php';

$scr = new ScreenNav(5, 8456, 'http://some/url');

ScrNavTestSetGet::get($scr);


//echo var_dump(ScreenNav::pageNo('part')) .PHP_EOL;