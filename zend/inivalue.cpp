/**
 *  IniValue.cpp
 *
 *  Class IniValue designed for extracting values from ini entries
 *
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Cast to a number
 *  @return uint64_t
 */
int64_t IniValue::numericValue() const
{
    return zend_ini_long(const_cast<char*>(_name.c_str()), _name.size(), _isorig);

}

/**
 *  Get access to the raw buffer for read operationrs.
 *  @return const char *
 */
const char* IniValue::rawValue() const
{
    return zend_ini_string(const_cast<char*>(_name.c_str()), _name.size(), _isorig);
}

/**
 *  Cast to a floating point
 *  @return double
 */
IniValue::operator double() const
{
    return zend_ini_double(const_cast<char*>(_name.c_str()), _name.size(), _isorig);
}

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  ini_val
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const IniValue &ini_val)
{
    return stream << static_cast<const char*>(ini_val);
}

/**
 *  End of namespace
 */
}


