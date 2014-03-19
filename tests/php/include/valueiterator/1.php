<?php

class SimpleClass {
    public    $cl0_float = 3.14;
    public    $cl0_str1  = 'public str1';
    private   $cl0_str2  = 'private str2';
    protected $cl0_str3  = 'protected str3';
}

class impIterator implements Iterator {
    private $position = 0;
    private $array = array(
        "firstElement",
        "secondElement",
        "lastelEment",
    );  

    public function __construct() {
        $this->position = 0;
    }

    function rewind() {
        $this->position = 0;
    }

    function current() {
        return $this->array[$this->position];
    }

    function key() {
        return 'key_' . $this->position;
    }

    function next() {
        ++$this->position;
    }

    function valid() {
        return isset($this->array[$this->position]);
    }
    
    function __destruct() {
        echo "~impIterator\n";
    }
}

class impIterAggr1 implements IteratorAggregate {
    public function getIterator() {
        return new ArrayIterator(new SimpleClass);
    }
    function __destruct() {
        echo "~impIterAggr1\n";
    }
}

class impIterAggr2 implements IteratorAggregate {
    public function getIterator() {
        return new impIterator();
    }
    function __destruct() {
        echo "~impIterAggr2\n";
    }
}













