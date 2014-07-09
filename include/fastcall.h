/**
 *  fastcall.h
 *
 *  This file holds some PHP functions implementation in C directly.
 *
 */

namespace Php {

    Value eval(const std::string &phpCode);
    inline bool is_a(const Value &obj, const std::string classname, bool allow_string = false) {
        return obj.is(classname, allow_string);
    }
    inline bool is_subclass_of(const Value &obj, const std::string classname, bool allow_string = true) {
        return obj.isSubClassOf(classname, allow_string);
    }
    bool class_exists(const std::string &classname, bool autoload = true);

}

