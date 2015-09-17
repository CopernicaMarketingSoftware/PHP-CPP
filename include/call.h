/**
 *  Call.h
 *
 *  This file holds a function to call a PHP function
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  List of functions that are available for use in PHP
 */
extern PHPCPP_EXPORT    bool  class_exists(const char *classname, size_t size, bool autoload = true);
inline PHPCPP_EXPORT    bool  class_exists(const char *classname, bool autoload = true) { return class_exists(classname, strlen(classname), autoload); }
inline PHPCPP_EXPORT    bool  class_exists(const std::string &classname, bool autoload = true) { return class_exists(classname.c_str(), classname.size(), autoload); }
extern PHPCPP_EXPORT    Value constant(const char *constant);
extern PHPCPP_EXPORT    Value constant(const char *constant, size_t size);
extern PHPCPP_EXPORT    Value constant(const std::string &constant);
extern PHPCPP_EXPORT    bool  define(const char *name, size_t size, const Value &value);
extern PHPCPP_EXPORT    bool  define(const char *name, const Value &value);
extern PHPCPP_EXPORT    bool  define(const std::string &name, const Value &value);
extern PHPCPP_EXPORT    bool  defined(const char *constant);
extern PHPCPP_EXPORT    bool  defined(const char *constant, size_t size);
extern PHPCPP_EXPORT    bool  defined(const std::string &constant);
extern PHPCPP_EXPORT    bool  dl(const char *filename, bool persistent = false);
inline PHPCPP_EXPORT    bool  dl(const std::string &filename, bool persistent = false) { return dl(filename.c_str(), persistent); }
inline PHPCPP_EXPORT    bool  dl(const Value &filename, bool persistent = false) { return dl(filename.rawValue(), persistent); }
extern PHPCPP_EXPORT    Value eval(const char *phpCode);
inline PHPCPP_EXPORT    Value eval(const std::string &phpCode) { return eval(phpCode.c_str()); }
extern PHPCPP_EXPORT    Value include(const char *filename);
inline PHPCPP_EXPORT    Value include(const std::string &filename) { return include(filename.c_str()); }
extern PHPCPP_EXPORT    Value include_once(const char *filename);
inline PHPCPP_EXPORT    Value include_once(const std::string &filename) { return include_once(filename.c_str()); }
inline PHPCPP_EXPORT    bool  is_a(const Value &obj, const char *classname, size_t size, bool allow_string = false) { return obj.instanceOf(classname, size, allow_string); }
inline PHPCPP_EXPORT    bool  is_a(const Value &obj, const char *classname, bool allow_string = false) { return is_a(obj, classname, strlen(classname), allow_string); }
inline PHPCPP_EXPORT    bool  is_a(const Value &obj, const std::string &classname, bool allow_string = false) { return is_a(obj, classname.c_str(), classname.size(), allow_string); }
inline PHPCPP_EXPORT    bool  is_subclass_of(const Value &obj, const char *classname, size_t size, bool allow_string = true) { return obj.derivedFrom(classname, size, allow_string); }
inline PHPCPP_EXPORT    bool  is_subclass_of(const Value &obj, const char *classname, bool allow_string = true) { return is_subclass_of(obj, classname, strlen(classname), allow_string); }
inline PHPCPP_EXPORT    bool  is_subclass_of(const Value &obj, const std::string &classname, bool allow_string = true) { return is_subclass_of(obj, classname.c_str(), classname.size(), allow_string); }
extern PHPCPP_EXPORT    Value require(const char *filename);
inline PHPCPP_EXPORT    Value require(const std::string &filename) { return require(filename.c_str()); }
extern PHPCPP_EXPORT    Value require_once(const char *filename);
inline PHPCPP_EXPORT    Value require_once(const std::string &filename) { return require_once(filename.c_str()); }
extern PHPCPP_EXPORT    Value set_exception_handler(const std::function<Value(Parameters &params)> &handler);
extern PHPCPP_EXPORT    Value set_error_handler(const std::function<Value(Parameters &params)> &handler, Error error = Error::All);
extern PHPCPP_EXPORT    Value error_reporting(Error error);
extern PHPCPP_EXPORT    const char *sapi_name();

/**
 *  Call a function in PHP
 *  @param  name        Name of the function to call
 *  @param  params      Variable number of parameters
 *  @return Value
 */
template <typename ...Params>
Value call(const char *name, Params&&... params)
{
    // the name can be turned into a Php::Value object, which implements
    // the operator () method to call it
    Value function(name);

    // invoke the operator ()
    return function(std::forward<Params>(params)...);
}

/**
 *  Long list of simply-forwarded function calls
 *
 *  Most functions in this list are forwarded to the call() method described
 *  above, which of course is slower than necessary, because they will have to
 *  pass the entire zend engine to look up the actual implementation, while a
 *  direct call the C implementation was possible too. The reason for this is
 *  that we are lazy - if you feel like looking up the actual implementation for
 *  each function in the PHP source, your support is more than welcome.
 *
 *  But since it is a stupid idea to call a PHP function from your extension
 *  anyway (that's what people write extension for: to get away from PHP and
 *  make the code run on the highway), it is not expected that these functions
 *  are going to be used very often anyway.
 */
inline PHPCPP_EXPORT    Value array_key_exists(const Value &key, const Value &array) { return array.contains(key); }
inline PHPCPP_EXPORT    Value array_key_exists(int key, const Value &array) { return array.contains(key); }
inline PHPCPP_EXPORT    Value array_key_exists(const char *key, const Value &array) { return array.contains(key); }
inline PHPCPP_EXPORT    Value array_key_exists(const std::string &key, const Value &array) { return array.contains(key); }
inline PHPCPP_EXPORT    Value array_keys(const Value &value) { return call("array_keys", value); }
inline PHPCPP_EXPORT    Value array_push(const Value &array, const Value &value) { return call("array_push", array, value); }
inline PHPCPP_EXPORT    Value array_values(const Value &value) { return call("array_values", value); }
inline PHPCPP_EXPORT    Value count(const Value &value) { return call("count", value); }
inline PHPCPP_EXPORT    Value echo(const char *input) { out << input; return nullptr; }
inline PHPCPP_EXPORT    Value echo(const std::string &input) { out << input; return nullptr; }
inline PHPCPP_EXPORT    Value empty(const Value &value) { return value.isNull() || !value.boolValue(); }
inline PHPCPP_EXPORT    Value empty(const HashMember<std::string> &member) { return !member.exists() || empty(member.value()); }
inline PHPCPP_EXPORT    Value empty(const HashMember<int> &member) { return !member.exists() || empty(member.value()); }
inline PHPCPP_EXPORT    Value is_array(const Value &value) { return value.isArray(); }
inline PHPCPP_EXPORT    Value strlen(const Value &value) { return call("strlen", value); }
inline PHPCPP_EXPORT    void  unset(const HashMember<std::string> &member) { member.unset(); }
inline PHPCPP_EXPORT    void  unset(const HashMember<int> &member) { member.unset(); }
inline PHPCPP_EXPORT    void  unset(const HashMember<Value> &member) { member.unset(); }

/**
 *  The 'ini_get' function returns an IniValue, so that it can also be used
 *  before the PHP engine is started.
 */
inline PHPCPP_EXPORT    IniValue ini_get(const char* name) { return IniValue(name, false); }
inline PHPCPP_EXPORT    IniValue ini_get_orig(const char* name) { return IniValue(name, true); }


/**
 *  The isset function conflicts with the 'isset' macro defined by the Zend engine
 */
#pragma push_macro("isset")
#undef isset

/**
 *  Define the isset function
 */
inline PHPCPP_EXPORT    Value isset(const Value &value) { return call("isset", value); }
inline PHPCPP_EXPORT    Value isset(const HashMember<std::string> &member) { return member.exists() && isset(member.value()); }
inline PHPCPP_EXPORT    Value isset(const HashMember<int> &member) { return member.exists() && isset(member.value()); }
inline PHPCPP_EXPORT    Value isset(const HashMember<Value> &member) { return member.exists() && isset(member.value()); }

/**
 *  Re-install the ISSET macro
 */
#pragma pop_macro("isset")

/**
 *  End of namespace
 */
}

