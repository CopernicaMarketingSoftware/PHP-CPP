/**
 *  Type.h
 *
 *  In this file an enumeration type is defined with all supported variable
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
enum class Type : unsigned char {
    Undefined       =   0,  // Variable is not set
    Null            =   1,  // Null will allow any type
    False           =   2,  // Boolean false
    True            =   3,  // Boolean true
    Numeric         =   4,  // Integer type
    Float           =   5,  // Floating point type
    String          =   6,  // A string obviously
    Array           =   7,  // An array of things
    Object          =   8,  // An object
    Resource        =   9,  // A resource
    Reference       =  10,  // Reference to another value (can be any type!)
    Constant        =  11,  // A constant value
    ConstantAST     =  12,  // I think an Abstract Syntax tree, not quite sure

    // "fake types", not quite sure what that means
    Bool            = 13,   // You will never get this back as a type
    Callable        = 14,   // I don't know why this is a "fake" type
};

/**
 *  End of namespace
 */
}
