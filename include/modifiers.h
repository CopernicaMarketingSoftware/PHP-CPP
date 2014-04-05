/**
 *  Modifiers.h
 *
 *  In this file an enumeration type is with the possible
 *  member modifiers
 *
 *  @author Martijn Otto
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  The modifiers are constants
 */
extern const int Static;
extern const int Abstract;
extern const int Final;
extern const int Public;
extern const int Protected;
extern const int Private;
extern const int Const;

/**
 *  Modifiers that are supported for methods and properties
 */
extern const int MethodModifiers;
extern const int PropertyModifiers;

/**
 *  End namespace
 */
}
