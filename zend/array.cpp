/**
 *  array.cpp
 *
 *  This file holds the implementation of the Php::Array functions
 * 
 *  @author tugrul <https://github.com/tugrul>
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Push a Php::Value to the array as stack
 *  @param  value
 */
int Array::push(Value val)
{
    Array *value = static_cast<Array*>(&val);

    Z_ADDREF_P(value->_val);
    
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_val), &value->_val, sizeof(zval *), NULL) == FAILURE) {
        Z_DELREF_P(value->_val); 
        Php::warning << "Cannot add element to the array as the next element is already occupied" << std::flush;
    }
    
    return count();
}

/**
 *  Pop a Php::Value from the array as stack
 */
Value Array::pop()
{
    zval **val;
    
    ulong numKey;
    char *strKey = NULL;
    uint strKeyLen = 0; 
    
    if (count() == 0) {
        return Value();
    }

    zend_hash_internal_pointer_end(Z_ARRVAL_P(_val));
    zend_hash_get_current_data(Z_ARRVAL_P(_val), (void **)&val);
    
    Value retVal(*val);
    
    zend_hash_get_current_key_ex(Z_ARRVAL_P(_val), &strKey, &strKeyLen, &numKey, 0, NULL);
    zend_hash_del_key_or_index(Z_ARRVAL_P(_val), strKey, strKeyLen, numKey, (strKey) ? HASH_DEL_KEY : HASH_DEL_INDEX);

    if (!strKeyLen && Z_ARRVAL_P(_val)->nNextFreeElement > 0 && numKey >= Z_ARRVAL_P(_val)->nNextFreeElement - 1) {
		Z_ARRVAL_P(_val)->nNextFreeElement = Z_ARRVAL_P(_val)->nNextFreeElement - 1;
	}
    
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(_val));
    
    return retVal;
}

/**
 *  Get keys of the array like PHP's array_keys
 */
Array Array::getKeys()
{
    Array retVal;
    HashPosition pos;
    zval **entry;
    
    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(_val), &pos);
    while (zend_hash_get_current_data_ex(Z_ARRVAL_P(_val), (void **)&entry, &pos) == SUCCESS) {

        ulong numKey;
        char *strKey;
        uint strKeyLen; 

        switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(_val), &strKey, &strKeyLen, &numKey, 0, &pos)) {
            case HASH_KEY_IS_STRING:
                retVal.push(std::string(strKey, strKeyLen)); 
                break; 

            case HASH_KEY_IS_LONG: 
                retVal.push((int32_t)numKey);
                break;
        }

		zend_hash_move_forward_ex(Z_ARRVAL_P(_val), &pos);
	}

    return retVal;
}

/**
 *  End of namespace
 */
}
