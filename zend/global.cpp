/**
 *  Global.cpp
 *
 *  Implementation for the global variable
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace Php {

/**
 *  Move constructor
 *  @param  global
 */
Global::Global(Global &&global) _NOEXCEPT :
    Value(std::move(global)),
    _name(global._name),
    _exists(global._exists)
{
    // remove from other global to avoid double free
    global._name = nullptr;
}

/**
 *  Constructor for non-existing var
 *
 *  @param  name    Name for the variable that does not exist
 */
Global::Global(const char *name) :
    Value(),
    _name(zend_string_init(name, ::strlen(name), 1)),
    _exists(false) {}

/**
 *  Alternative constructor for non-existing var
 *  @param  name
 */
Global::Global(const std::string &name) :
    Value(),
    _name(zend_string_init(name.data(), name.size(), 1)),
    _exists(false) {}

/**
 *  Constructor to wrap zval for existing global bar
 *  @param  name
 *  @param  val
 */
Global::Global(const char *name, struct _zval_struct *val) :
    Value(val, true),
    _name(zend_string_init(name, ::strlen(name), 1)),
    _exists(true) {}

/**
 *  Alternative constructor to wrap zval
 *  @param  name
 *  @param  val
 */
Global::Global(const std::string &name, struct _zval_struct *val) :
    Value(val, true),
    _name(zend_string_init(name.data(), name.size(), 1)),
    _exists(true) {}

/**
 *  Destructor
 */
Global::~Global()
{
    // release the string
    if (_name) zend_string_release(_name);
}

/**
 *  Function that is called when the value is updated
 *  @return Value
 */
Global &Global::update()
{
    // skip if the variable already exists
    if (_exists) return *this;

    // add one extra reference because the variable now is a global var too
    Z_TRY_ADDREF_P(_val);

    // add the variable to the globals
    zend_symtable_update_ind(&EG(symbol_table), _name, _val);

    // remember that the variable now exists
    _exists = true;

    // done
    return *this;
}

/**
 *  End of namespace
 */
}

