<?php
// Run: php -d extension=$PWD/traverse_leak.so test.php
// Expected (fixed): "leaked: 0 bytes" per 1000 iterations; unpatched PHP-CPP leaks one zend_object_iterator per call.
class Counter implements IteratorAggregate {
    public static int $destructed = 0;
    public function getIterator(): Iterator { return new ArrayIterator([1, 2, 3]); }
    public function __destruct() { self::$destructed++; }
}

function run(string $label, callable $make, int $n = 20000): void {
    gc_collect_cycles();
    $before = memory_get_usage();
    for ($i = 0; $i < $n; $i++) {
        $it = $make();
        traverse_count($it);
        unset($it);
    }
    gc_collect_cycles();
    printf("%-34s leaked: %8d bytes after %d calls\n", $label, memory_get_usage() - $before, $n);
}

run('ArrayIterator', fn() => new ArrayIterator([1, 2, 3]));
run('IteratorAggregate (userland)', fn() => new Counter());
run('Generator', function () { return (function () { yield 1; yield 2; yield 3; })(); });
printf("Counter::__destruct calls: %d (expected 20000)\n", Counter::$destructed);
