/**
 *  HashMember.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h" 
 
/**
 *  Set up namespace
 */
namespace Php {
 
/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const HashMember<int> &value)
{
    return stream << value.value();
}

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  value
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const HashMember<std::string> &value)
{
    return stream << value.value();
}

/**
 *  End of namespace
 */
}

