<?php
// PHP 8 calls the compare handler of an object as soon as either operand is an object, so ClassImpl::compare()
// is invoked with an integer, string, array or null as one of its operands. Used to dereference that value as an object (segfault).
error_reporting(E_ALL & ~E_WARNING & ~E_NOTICE);

$failed = false;
function check(string $label, $actual, $expected): void
{
    global $failed;
    $ok = $actual === $expected;
    if (!$ok) $failed = true;
    printf("%s %-40s %s\n", $ok ? 'ok    ' : 'FAILED', $label, $ok ? '' : 'got ' . var_export($actual, true));
}

$plain = new Plain();
$comparable = new Comparable(5);

// the integer must never be read as an object: an object without a conversion never equals it
check('in_array(int, [Plain])', in_array(440320, [$plain]), false);
check('Plain == int', $plain == 5, false);
check('int == Plain', 5 == $plain, false);
check('Plain == string', $plain == 'abc', false);
check('Plain == null', $plain == null, false);
check('Plain == array', $plain == [1], false);
check('in_array(int, [Comparable])', in_array(440320, [$comparable]), false);
check('Comparable == int', $comparable == 5, false);
check('int == Comparable', 5 == $comparable, false);
check('array_search(int, [Comparable])', array_search(440320, [$comparable]), false);

// comparing two objects of the same class is unchanged
check('Plain == Plain', new Plain() == new Plain(), true);
check('Comparable == Comparable (equal)', new Comparable(3) == new Comparable(3), true);
check('Comparable < Comparable', new Comparable(3) < new Comparable(4), true);
check('Comparable > Comparable', new Comparable(4) > new Comparable(3), true);
check('Comparable == Comparable (differs)', new Comparable(3) == new Comparable(4), false);

exit($failed ? 1 : 0);
