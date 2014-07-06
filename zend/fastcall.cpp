/**
 *  fastcall.cpp
 *
 *  This file holds some PHP functions implementation in C directly.
 *
 */

#include "includes.h"

namespace Php {

    Value eval(const std::string &phpCode) {
        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        // the current exception
        zval* oldException = EG(exception);

        // the return zval
        zval* retval = nullptr;
        if (zend_eval_stringl_ex((char *)phpCode.c_str(), (int32_t)phpCode.length(), retval, (char *)"", 1 TSRMLS_CC) != SUCCESS)
        {
            // throw an exception, php couldn't evaluate code
            throw Exception("PHP couldn't evaluate: " + phpCode);

            // unreachable, but let's return at least something to prevent compiler warnings
            return nullptr;
        }
        else
        {
            // was an exception thrown inside the function? In that case we throw a C++ new exception
            // to give the C++ code the chance to catch it
            if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

            // no (additional) exception was thrown
            return retval ? Value(retval) : nullptr;
        }
    }

    bool class_exists(const std::string &classname, bool autoload) {
        // we need the tsrm_ls variable
        TSRMLS_FETCH();

        zend_class_entry **ce;
        int found;
        const char * str = classname.c_str();
        int32_t len = (int32_t)classname.length();


        if (autoload) {
            char lc_name[len + 1];
            zend_str_tolower_copy(lc_name, str, len);

            char *name = lc_name;
            if (lc_name[0] == '\\') {
                name = &lc_name[1];
                --len;
            }

            found = zend_hash_find(EG(class_table), name, len + 1, (void **) &ce);
            return (found == SUCCESS && !(((*ce)->ce_flags & (ZEND_ACC_INTERFACE | ZEND_ACC_TRAIT)) > ZEND_ACC_EXPLICIT_ABSTRACT_CLASS));
        }

        if (zend_lookup_class(str, len, &ce TSRMLS_CC) == SUCCESS) {
            return (((*ce)->ce_flags & (ZEND_ACC_INTERFACE | (ZEND_ACC_TRAIT - ZEND_ACC_EXPLICIT_ABSTRACT_CLASS))) == 0);
        }
        return false;
    }

}