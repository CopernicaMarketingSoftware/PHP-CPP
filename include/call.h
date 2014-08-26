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
extern bool  class_exists(const char *classname, size_t size, bool autoload = true);
inline bool  class_exists(const char *classname, bool autoload = true) { return class_exists(classname, strlen(classname), autoload); }
inline bool  class_exists(const std::string &classname, bool autoload = true) { return class_exists(classname.c_str(), classname.size(), autoload); }
extern Value eval(const std::string &phpCode);
inline bool  is_a(const Value &obj, const char *classname, size_t size, bool allow_string = false) { return obj.instanceOf(classname, size, allow_string); }
inline bool  is_a(const Value &obj, const char *classname, bool allow_string = false) { return is_a(obj, classname, strlen(classname), allow_string); }
inline bool  is_a(const Value &obj, const std::string &classname, bool allow_string = false) { return is_a(obj, classname.c_str(), classname.size(), allow_string); }
inline bool  is_subclass_of(const Value &obj, const char *classname, size_t size, bool allow_string = true) { return obj.derivedFrom(classname, size, allow_string); }
inline bool  is_subclass_of(const Value &obj, const char *classname, bool allow_string = true) { return is_subclass_of(obj, classname, strlen(classname), allow_string); }
inline bool  is_subclass_of(const Value &obj, const std::string &classname, bool allow_string = true) { return is_subclass_of(obj, classname.c_str(), classname.size(), allow_string); }


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
inline Value array_key_exists(const Value &key, const Value &array) { return array.contains(key); }
inline Value array_key_exists(int key, const Value &array) { return array.contains(key); }
inline Value array_key_exists(const char *key, const Value &array) { return array.contains(key); }
inline Value array_key_exists(const std::string &key, const Value &array) { return array.contains(key); }
inline Value array_keys(const Value &value) { return call("array_keys", value); }
inline Value array_push(const Value &array, const Value &value) { return call("array_push", array, value); }
inline Value array_values(const Value &value) { return call("array_values", value); }
inline Value count(const Value &value) { return call("count", value); }
inline Value echo(const char *input) { out << input; return nullptr; }
inline Value echo(const std::string &input) { out << input; return nullptr; }
inline Value empty(const Value &value) { return value.isNull() || !value.boolValue(); }
inline Value empty(const HashMember<std::string> &member) { return !member.exists() || empty(member.value()); }
inline Value empty(const HashMember<int> &member) { return !member.exists() || empty(member.value()); }
inline Value is_array(const Value &value) { return value.isArray(); }
inline Value strlen(const Value &value) { return call("strlen", value); }
inline void  unset(const HashMember<std::string> &member) { member.unset(); }
inline void  unset(const HashMember<int> &member) { member.unset(); }
inline void  unset(const HashMember<Value> &member) { member.unset(); }

/**
 *  The 'ini_get' function returns an IniValue, so that it can also be used
 *  before the PHP engine is started.
 */
inline IniValue ini_get(const char* name) { return IniValue(name, false); }
inline IniValue ini_get_orig(const char* name) { return IniValue(name, true); }


/**
 *  The isset function conflicts with the 'isset' macro defined by the Zend engine
 */
#pragma push_macro("isset")
#undef isset

/**
 *  Define the isset function
 */
inline Value isset(const Value &value) { return call("isset", value); }
inline Value isset(const HashMember<std::string> &member) { return member.exists() && isset(member.value()); }
inline Value isset(const HashMember<int> &member) { return member.exists() && isset(member.value()); }
inline Value isset(const HashMember<Value> &member) { return member.exists() && isset(member.value()); }

/**
 *  Re-install the ISSET macro
 */
#pragma pop_macro("isset")

/**
 *  End of namespace
 */
}

