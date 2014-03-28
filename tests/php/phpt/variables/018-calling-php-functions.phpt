--TEST--
calling-php-callable
--DESCRIPTION--
call callable object of class from user space
call method of class from user space
--SKIPIF--
<?php if (!extension_loaded("extension_for_tests")) print "skip"; ?>
--FILEEOF--
<?php


class CallableClass {
    public function __construct($a){ $this->a = $a;}
    public function __invoke($b){ echo "CallableClass({$this->a})::__invoke($b)";}
    private $a = "";
}

class usrspClass {
  function __construct($a){ $this->a = $a;}
  function someMethod($b) {echo "usrspClass({$this->a})::someMethod($b)";}
  private $a = "";
}


TestVariables\fnFromUserSpace2();


--EXPECT--
2014-03-28 21:22:15
2014-03-28 21:22:15
usrspClass(Mount Meru)::someMethod(is in the Arctics)
CallableClass(Arctics around mount Meru)::__invoke(is the birthplace of the Hyperboreans)