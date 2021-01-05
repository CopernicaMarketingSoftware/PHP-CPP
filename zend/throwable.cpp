/**
 *  Throwable.cpp
 * 
 *  Implementation file for the Throwable class
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Helper function converts an object to a string
 *  @return std::string
 */
static std::string convert(zend_object *object)
{
    // the result value from zend and the object zval
    zval tmp, properties;

    // retrieve the object properties
    ZVAL_OBJ(&properties, object);

    // retrieve the message, filename, error code and line number
#if PHP_VERSION_ID < 80000
    auto message = zval_get_string(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("message"), 1, &tmp));
#else
    auto message = zval_get_string(zend_read_property(Z_OBJCE(properties), object, ZEND_STRL("message"), 1, &tmp));
#endif

    // copy message to a string
    std::string result(ZSTR_VAL(message), ZSTR_LEN(message));

    // clean up message string
    zend_string_release(message);
    
    // done
    return result;
}

/**
 *  Another protected constructor
 *  @param  object
 */
Throwable::Throwable(zend_object *object) : std::runtime_error(convert(object))
{
    // the result value from zend and the object zval
    zval result, properties;

    // retrieve the object properties
    ZVAL_OBJ(&properties, object);

    // retrieve the error code
#if PHP_VERSION_ID < 80000
    _code = zval_get_long(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("code"), 1, &result));
#else
    _code = zval_get_long(zend_read_property(Z_OBJCE(properties), object, ZEND_STRL("code"), 1, &result));
#endif
}

/**
 *  End of namespace
 */
}

