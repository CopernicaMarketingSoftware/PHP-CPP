/**
 *  Test extension for TraverseIterator: iterates any Traversable that is passed as a Php::Value
 *  and returns the number of elements.
 */
#include <phpcpp.h>

static Php::Value traverse_count(Php::Parameters &params)
{
    int64_t count = 0;

    // Php::Value::begin() creates a TraverseIterator for Traversable objects
    for (auto &iter : params[0]) { (void)iter; ++count; }

    return count;
}

extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("traverse_release", "1.0");
        extension.add<traverse_count>("traverse_count");
        return extension;
    }
}
