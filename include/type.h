/**
 *  Type.h
 *
 *  In this file an enumeration type is defined with all supporteded variable
 *  types.
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
 *  The values are the same as the ones used internally in Zend
 */
typedef enum _Type {
    nullType            =   0,
    longType            =   1,
    decimalType         =   2,
    boolType            =   3,
    arrayType           =   4,
    objectType          =   5,
    stringType          =   6,
    resourceType        =   7,
    constantType        =   8,
    constantArrayType   =   9,
    callableType        =  10
} Type;

/**
 *  End of namespace
 */
}

