/**
 *  fastcall.h
 *
 *  This file holds some PHP functions implementation in C directly.
 *
 */

namespace Php {

    /**
     * Get the value of a constant
     *  @param  constant_name
     *  @return value
     */
    Php::Value constant(const std::string &constant_name);
    /**
     * Define a new constant
     *  @param  constant_name
     *  @param  value
     *  @param  case_insensitive
     *  @return bool
     */
    bool define(const std::string &constant_name, const Php::Value &value, bool case_insensitive = false);
    /**
     * Check whether a constant exists
     *  @param  constant_name
     *  @return bool
     */
    bool defined(const std::string &constant_name);
}

