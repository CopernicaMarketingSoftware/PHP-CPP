/**
 *  Ini.cpp
 *
 *  Implementation for ....
 *
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Filling ini_entries
 *  @param  zend_ini_entry *ini_entry, int module_number
 *  @param  int module_number
 */
void Ini::fill(zend_ini_entry *ini_entry, int module_number)
{
    ini_entry->module_number     = module_number;
    ini_entry->modifiable        = static_cast<int>(this->_place);
    ini_entry->name              = const_cast<char*>(this->_name.c_str());
    ini_entry->name_length       = this->_name.size()+1;
    ini_entry->on_modify         = OnUpdateString;
    ini_entry->mh_arg1           = nullptr;
    #ifdef ZTS
        ini_entry->mh_arg2       = (void *) &phpcpp_globals_id;
    #else
        ini_entry->mh_arg2       = (void *) &phpcpp_globals;
    #endif
    ini_entry->mh_arg3           = nullptr;
    ini_entry->value             = const_cast<char*>(this->_value.c_str());
    ini_entry->value_length      = this->_value.size();
    if( this->_orig_empty) 
    {
        ini_entry->orig_value     = nullptr;
        ini_entry->orig_value_length = 0;
    } 
    else 
    {
        ini_entry->orig_value     = const_cast<char*>(this->_orig.c_str());
        ini_entry->orig_value_length = this->_orig.size();
    }
    ini_entry->orig_modifiable   = 0;
    ini_entry->modified          = 0;
    ini_entry->displayer         = nullptr;
}


/**
 *  Cast to a number
 *  @return uint64_t
 */
int64_t IniValue::numericValue() const
{
    return zend_ini_long(const_cast<char*>(_name.c_str()), _name.size()+1, _isorig);

}

/**
 *  Get access to the raw buffer for read operationrs.
 *  @return const char *
 */
const char* IniValue::rawValue() const
{
    return zend_ini_string(const_cast<char*>(_name.c_str()), _name.size()+1, _isorig);
}

/**
 *  Cast to a floating point
 *  @return double
 */
IniValue::operator double() const
{
    return zend_ini_double(const_cast<char*>(_name.c_str()), _name.size()+1, _isorig);
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


