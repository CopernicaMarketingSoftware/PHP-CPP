<?php
// TraverseIterator must release the Zend iterator it creates (zend_iterator_dtor). Calling the iterator's dtor
// function directly leaves the iterator allocated (and, for IteratorAggregate, keeps the object it was created from alive).

class Aggregate implements IteratorAggregate
{
    public static int $destructed = 0;
    public function getIterator(): Iterator { return new ArrayIterator([1, 2, 3]); }
    public function __destruct() { self::$destructed++; }
}

$calls = 20000;
$failed = false;

function check(string $label, callable $make, int $calls): bool
{
    // warm up, so that one-time allocations do not count
    for ($i = 0; $i < 100; $i++) { $it = $make(); traverse_count($it); unset($it); }

    gc_collect_cycles();
    $before = memory_get_usage();

    for ($i = 0; $i < $calls; $i++) {
        $it = $make();
        if (traverse_count($it) !== 3) { echo "FAILED $label: wrong element count\n"; return false; }
        unset($it);
    }

    gc_collect_cycles();
    $leaked = memory_get_usage() - $before;

    // one leaked zend_object_iterator per call is at least 56 bytes per call, so this threshold is far below that
    $ok = $leaked < 65536;
    printf("%s %-24s leaked %d bytes after %d calls\n", $ok ? 'ok    ' : 'FAILED', $label, $leaked, $calls);
    return $ok;
}

$failed |= !check('ArrayIterator', fn() => new ArrayIterator([1, 2, 3]), $calls);
$failed |= !check('IteratorAggregate', fn() => new Aggregate(), $calls);
$failed |= !check('Generator', fn() => (function () { yield 1; yield 2; yield 3; })(), $calls);

// every aggregate (and its generated iterator) must be destructed again: 100 warm-up calls + the measured calls
$expected = 100 + $calls;
if (Aggregate::$destructed !== $expected) {
    printf("FAILED Aggregate destructors: %d of %d\n", Aggregate::$destructed, $expected);
    $failed = true;
}

exit($failed ? 1 : 0);
