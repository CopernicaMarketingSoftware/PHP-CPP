<?php
namespace NamespaceExample;

/**
  *  The namespace in Zend is only a part of the name defined in it a class or function.
  *  see README.md
  *  you can also run `php --re NamespaceExampleExt`
  */


echo '<pre>';
var_dump(new TestClass());

echo TestFunc();

$refl = new \ReflectionClass('NamespaceExample\TestClass');
echo "\nNamespaceName: ", $refl->getNamespaceName();

echo "\nTestClass inNamespace: " ;
var_export($refl->inNamespace());



echo '</pre>';
