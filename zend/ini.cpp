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
void Ini::fill(zend_ini_entry_def *ini_entry, int module_number)
{
    ini_entry->modifiable       = static_cast<int>(_place);
    ini_entry->name             = _name.data();
    ini_entry->name_length      = _name.size();
    ini_entry->on_modify        = OnUpdateString;
    ini_entry->mh_arg1          = nullptr;
    #ifdef ZTS
        ini_entry->mh_arg2      = (void *) &phpcpp_globals_id;
    #else
        ini_entry->mh_arg2      = (void *) &phpcpp_globals;
    #endif
    ini_entry->mh_arg3          = nullptr;
    ini_entry->value            = _value.data();
    ini_entry->value_length     = _value.size();
    ini_entry->displayer        = nullptr;
}


/**
 *  End of namespace
 */
}


