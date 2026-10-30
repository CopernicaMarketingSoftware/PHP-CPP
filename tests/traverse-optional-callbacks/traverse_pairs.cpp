/**
 *  Test extension for TraverseIterator: iterates any Traversable that is passed as a Php::Value
 *  and returns a list with a [key, value] pair for every element.
 */
#include <phpcpp.h>

static Php::Value traverse_pairs(Php::Parameters &params)
{
    Php::Array result;

    // Php::Value::begin() creates a TraverseIterator for Traversable objects
    for (auto &iter : params[0])
    {
        Php::Array pair;
        pair[0] = iter.first;
        pair[1] = iter.second;
        result[result.size()] = pair;
    }

    return result;
}

extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("traverse_pairs", "1.0");
        extension.add<traverse_pairs>("traverse_pairs");
        return extension;
    }
}
