<?php

/**
  *  For functionality testing. You can then delete.
  *  
  */

class cl0 {
    public    $cl0_float = 3.14;
    public    $cl0_str1  = 'public str1';
    private   $cl0_str2  = 'private str2';
    protected $cl0_str3  = 'protected str3';
}

class cl1 extends cl0  implements arrayaccess
{
    static $cl1_static = 'static prop';
    
    const  CL1_CONST   = 'const prop';
    
    public $cl1_num   = 45615;
    public $cl1_str   = "Public Prop";
    
    private $cl1_pp1   = "Private Prop1";
    private $cl1_pp2   = "Private Prop2";
    
    protected $cl1_prp1   = "Protected Prop1";
    protected $cl1_prp2   = "Protected Prop2";
    
    public function fn($a) {
        echo $a;
    }
    
    function __toString() {
        return 'I\'m class cl1';
    }
    
    public function offsetSet($offset, $value) {
        if (!is_null($offset)) {
            $this->$offset = $value;
        }
    }
    public function offsetExists($offset) {
        return isset($this->$offset);
    }
    public function offsetUnset($offset) {
        unset($this->$offset);
    }
    public function offsetGet($offset) {
        return isset($this->$offset) ? $this->$offset : null;
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

$arr = new cl1();

$arr[5] = 55;
$arr['strstr'] = 'strstrstrstrstrstr';

//$arr = new emptyClass();
//$arr = array();


$q = new MyClass();

var_export($arr);

//$q->loopArray($arr);

// Works for objects and arrays
$q->loopObject($arr);
//$q->loopObject($arr);

