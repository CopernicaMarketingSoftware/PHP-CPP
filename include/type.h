/**
 *  @file type.h
 *
 *  This file provides symbols denoting the supported types within the Php
 *  environment
 *
 *  The symbols are scoped to prevent misuse and namespace pollution
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Supported types for variables
 *
 *  The values are the same as the ones used internally in Zend
 */
enum class PHPCPP_EXPORT Type : unsigned char {
    Null            =   0, // Null will allow any type
    Numeric         =   1,
    Float           =   2,
    Bool            =   3,
    Array           =   4,
    Object          =   5,
    String          =   6,
    Resource        =   7,
    Constant        =   8,
    ConstantArray   =   9,
    Callable        =  10
};

/**
 *  End of namespace
 */
}
