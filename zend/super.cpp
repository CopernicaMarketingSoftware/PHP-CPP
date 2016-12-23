/**
 *  Super.cpp
 *
 *  @copyright 2014 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */
#include "includes.h"
#include "string.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  A number of super-globals are always accessible
 */
Super POST      ("_POST");
Super GET       ("_GET");
Super COOKIE    ("_COOKIE");
Super SERVER    ("_SERVER");
Super ENV       ("_ENV");
Super FILES     ("_FILES");
Super REQUEST   ("_REQUEST");

/**
 * Calls @c zend_is_auto_global() if necessary and looks up this superglobal in the symbol table
 * @internal
 */
struct _zval_struct* Super::resolve()
{
    if (!_ziag_called) {
        // call zend_is_auto_global to ensure that the just-in-time globals are loaded
        // this needs to be done only once
        zend_is_auto_global(String(_name));
        _ziag_called = true;
    }

    return zend_hash_str_find(&EG(symbol_table), _name, std::strlen(_name));
}

/**
 *  Convert object to a value
 *  @return Value
 */
Value Super::value()
{
    zval* v = resolve();
    return v ? Value(v) : Value();
}

/**
 * Returns @c SUPERGLOBAL[key].
 * If there is no @c key index, it gets created
 */
Value Super::getKey(const char* key, std::size_t len)
{
    zval* arr = resolve();
    if (arr && Z_TYPE_P(arr) == IS_ARRAY) {
        zval* v = zend_symtable_str_find(Z_ARRVAL_P(arr), key, len);
        if (!v) {
            zval empty;
            ZVAL_NULL(&empty);
            v = zend_symtable_str_update(Z_ARRVAL_P(arr), key, len, &empty);
        }

        return Value(v, true);
    }

    return Value();
}

/**
 *  End namespace
 */
}

