// Minimal PHP-CPP extension: iterates any Traversable passed as a Php::Value (the same code path as
// IdSet::fromNestedArray() in our production extension) and returns the number of elements.
#include <phpcpp.h>

static Php::Value traverse_count(Php::Parameters &params)
{
    int64_t count = 0;
    for (auto &iter : params[0]) { (void)iter; ++count; }   // Php::Value::begin() -> TraverseIterator for Traversable objects
    return count;
}

extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("traverse_leak", "1.0");
        extension.add<traverse_count>("traverse_count");
        return extension;
    }
}
