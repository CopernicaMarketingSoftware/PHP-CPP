<?php

/**
  *  For debugging tests
  *  
  */


function out($obj) {
    
    
    $cl_name =  is_object($obj) ?
                    get_class($obj) :
                    (is_array($obj) ? "Array" : "----");
                    
    echo "++++-" , $cl_name , "-++++\n";
    
    echo "\x1b[1;31m";
    echo "\n Native iterate:\n\n";
    foreach($obj as $k => $v) {
        echo "$k\t=>\t$v\n";
    }
    echo "\x1b[0m";

    
    TestValueIterator\loopValue($obj);
}



require './include/valueiterator/1.php';

#$arr  = array('qwe' => 'qweqweqweqw',5,'asd' => '«£¥§©®°±¶⅐⅒⅓⅘⅞Ⅻↆ❄❅❆⚑⚐⌛⌚〰»', 'zxccvx' => 'sdfsecvyh6bug6yfty',);
#$obj  = (object)$arr;
#$smpl = new SimpleClass;
#$it   = new impIterator;
#$iag1 = new impIterAggr1;
$iag2 = new impIterAggr2;


//out($arr);
#out($obj);
#out($smpl);
TestValueIterator\loopValue($iag2);
#out($it);
#out($iag1);
#out($iag2);











