<?php

/**
  *  For functionality testing. You can then delete.
  *  
  */

class cl1 {
    public $qwe = 45615;
    public $asd = "asdasdasd";
    public $zxcv = "Привет!"; // check UTF-8 chars
    public function fn($a) {
        echo $a;
    }
    function __destruct(){
        echo 'cl1::__destruct';
    }
    
    function __toString() {
        return 'I\'m class cl1';
    }
    
}

class emptyClass {}

$arr = array(
        'qwe'   => 'qweqweqweqw',
        'asd'   => 'Привет!', // check UTF-8 chars
        'zxccvx' => 'sdfsecvyh6bug6yfty',
        1=>2,
        '2'=>2,
        44,
        new cl1(),
        '%'=>'%$%$%',
    );
//$arr = array(5,17,'qwe' => 'qweqweqweqw',4=>88,'17'=>'170','1'=>4, new cl1());
//$arr = array(3.14,2.7,11,0,500);
//$arr = array();
//$arr = new cl1();

$q = new MyClass();

var_export($arr);

//$q->loopArray($arr);

// Works for objects and arrays
$q->loopObject($arr);

$q->loopObject(new emptyClass());


/*
// Validation removal (i.e. do I need to use zval_add_ref(value);)
echo "\nunset(\$arr):";
unset($arr);
echo "\nunset(\$q):";
unset($q);
*/



