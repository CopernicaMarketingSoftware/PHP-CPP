<?php
// The rewind and get_current_key callbacks of a Zend iterator are optional (NULL for the DOM iterators and for
// IntlBreakIterator): the engine's foreach checks for them, TraverseIterator used to call them unconditionally (crash on a NULL pointer).
$failed = false;
function check(string $label, $actual, $expected): void
{
    global $failed;
    $ok = $actual === $expected;
    if (!$ok) $failed = true;
    printf("%s %-36s %s\n", $ok ? 'ok    ' : 'FAILED', $label, $ok ? '' : 'got ' . json_encode($actual) . ' expected ' . json_encode($expected));
}

$doc = new DOMDocument();
$doc->loadXML('<root><a id="1"/><b id="2"/><c id="3"/></root>');

// DOM iterators have no rewind callback
$names = array_map(fn($pair) => $pair[1]->nodeName, traverse_pairs($doc->documentElement->childNodes));
check('DOMNodeList elements', $names, ['a', 'b', 'c']);
check('DOMNodeList keys', array_column(traverse_pairs($doc->documentElement->childNodes), 0), [0, 1, 2]);
check('DOMNodeList element count', count(traverse_pairs($doc->getElementsByTagName('*'))), 4);
check('DOMNodeList names (live list)', array_map(fn($pair) => $pair[1]->nodeName, traverse_pairs($doc->getElementsByTagName('*'))), ['root', 'a', 'b', 'c']);
check('DOMNodeList empty', traverse_pairs($doc->getElementsByTagName('missing')), []);

$attributes = traverse_pairs($doc->documentElement->firstChild->attributes);
check('DOMNamedNodeMap pairs', array_map(fn($pair) => [$pair[0], $pair[1]->nodeValue], $attributes), [['id', '1']]);

// iterators with all callbacks are unchanged
check('ArrayIterator pairs', traverse_pairs(new ArrayIterator(['x' => 1, 'y' => 2])), [['x', 1], ['y', 2]]);
check('Generator pairs', traverse_pairs((function () { yield 'k' => 'v'; yield 'l' => 'w'; })()), [['k', 'v'], ['l', 'w']]);

// IntlBreakIterator has no rewind callback either
if (class_exists('IntlBreakIterator')) {
    $it = IntlBreakIterator::createWordInstance('en');
    $it->setText('hello big world');
    check('IntlBreakIterator boundaries', array_column(traverse_pairs($it), 1), [0, 5, 6, 9, 10, 15]);
    check('IntlBreakIterator keys', array_column(traverse_pairs($it), 0), [0, 1, 2, 3, 4, 5]);
}

exit($failed ? 1 : 0);
