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
inline Value array_keys(const Value &value)                 { return call("array_keys", value); }
inline Value array_values(const Value &value)               { return call("array_values", value); }
inline Value empty(const Value &value)                      { return value.isNull() || !value.boolValue(); }
inline Value empty(const HashMember<std::string> &member)   { return !member.exists() || empty(member.value()); }
inline Value empty(const HashMember<int> &member)           { return !member.exists() || empty(member.value()); }
//inline Value isset(const Value &value)                      { return call("isset", value); }
//inline Value isset(const HashMember<std::string> &member)   { return member.exists() && isset(member.value()); }
//inline Value isset(const HashMember<int> &member)           { return member.exists() && isset(member.value()); }

/**
 *  End of namespace
 */
}

