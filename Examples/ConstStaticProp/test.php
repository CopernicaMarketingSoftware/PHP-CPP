<?php
        
    var_dump(MyTestClass::version); // print: string(11) "v0.01-alpha"
    
    var_dump(MyTestClass::PI); // print: float(3.14159265)
    
    var_dump(MyTestClass::IMISNULL); // print: NULL
    
    var_dump(MyTestClass::$exp); // print:  float(2.71828182846)
    
    /**
      *  Fatal error!
      *  Private PHP constructor. You can't instance object of MyTestClass.
      */
    //$mytest = new MyTestClass();