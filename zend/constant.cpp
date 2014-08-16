/**
 *  constant.cpp
 *
 *  This file holds the implementation for the Php::constant(),
 *  Php::define() and Php::defiend() functions
 *
 *  @author andot <https://github.com/andot>
 */

#include "includes.h"

namespace Php {

    /**
     * Get the value of a constant
     *  @param  constant_name
     *  @return value
     */
    Php::Value constant(const std::string &constant_name) {
        Php::Value result;
        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        if (!zend_get_constant_ex(constant_name.c_str(), constant_name.length(), result._val, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Couldn't find constant %s", constant_name.c_str());
            return nullptr;
        }

        return result;
    }
    /**
     * Define a new constant
     *  @param  constant_name
     *  @param  value
     *  @param  case_insensitive
     *  @return bool
     */
    bool define(const std::string &constant_name, const Php::Value &value, bool case_insensitive) {
        char *name = (char *)constant_name.c_str();
        int name_len = constant_name.length();
        zval *val = value._val;
        zval *val_free = nullptr;
        zend_constant c;
        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        /* class constant, check if there is name and make sure class is valid & exists */
        if (zend_memnstr(name, (char *)"::", sizeof("::") - 1, name + name_len)) {
            zend_error(E_WARNING, "Class constants cannot be defined or redefined");
            return false;
        }
    repeat:
        switch (Z_TYPE_P(val)) {
            case IS_LONG:
            case IS_DOUBLE:
            case IS_STRING:
            case IS_BOOL:
            case IS_RESOURCE:
            case IS_NULL:
                break;
            case IS_OBJECT:
                if (!val_free) {
                    if (Z_OBJ_HT_P(val)->get) {
                        val_free = val = Z_OBJ_HT_P(val)->get(val TSRMLS_CC);
                        goto repeat;
                    } else if (Z_OBJ_HT_P(val)->cast_object) {
                        ALLOC_INIT_ZVAL(val_free);
                        if (Z_OBJ_HT_P(val)->cast_object(val, val_free, IS_STRING TSRMLS_CC) == SUCCESS) {
                            val = val_free;
                            break;
                        }
                    }
                }
                /* no break */
            default:
                zend_error(E_WARNING,"Constants may only evaluate to scalar values");
                if (val_free) {
                    zval_ptr_dtor(&val_free);
                }
                return false;
        }
        c.value = *val;
        zval_copy_ctor(&c.value);
        if (val_free) {
            zval_ptr_dtor(&val_free);
        }
        c.flags = !case_insensitive; /* non persistent */
        c.name = IS_INTERNED(name) ? name : zend_strndup(name, name_len);
        if(c.name == NULL) {
            return false;
        }
        c.name_len = name_len + 1;
        c.module_number = PHP_USER_CONSTANT;
        if (zend_register_constant(&c TSRMLS_CC) == SUCCESS) {
            return true;
        }
        else {
            return false;
        }
    }
    /**
     * Check whether a constant exists
     *  @param  constant_name
     *  @return bool
     */
    bool defined(const std::string &constant_name) {
        zval c;
        // we need the tsrm_ls variable
        TSRMLS_FETCH();
        if (zend_get_constant_ex(constant_name.c_str(), constant_name.length(), &c, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
            zval_dtor(&c);
            return true;
        } else {
            return false;
        }
    }
}
