/**
 *  Test extension for ClassImpl::compare: one class without a __compare() method, and one with.
 */
#include <phpcpp.h>

class Plain : public Php::Base
{
};

class Comparable : public Php::Base
{
public:
    int value = 0;
    void __construct(Php::Parameters &params) { value = params[0]; }
    int __compare(const Comparable &that) const { return value < that.value ? -1 : (value > that.value ? 1 : 0); }
};

extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("compare_nonobject", "1.0");

        Php::Class<Plain> plain("Plain");
        extension.add(std::move(plain));

        Php::Class<Comparable> comparable("Comparable");
        comparable.method<&Comparable::__construct>("__construct");
        extension.add(std::move(comparable));

        return extension;
    }
}
