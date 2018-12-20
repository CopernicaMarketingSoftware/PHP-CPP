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
extern PHPCPP_EXPORT const int Static;
extern PHPCPP_EXPORT const int Abstract;
extern PHPCPP_EXPORT const int Final;
extern PHPCPP_EXPORT const int Public;
extern PHPCPP_EXPORT const int Protected;
extern PHPCPP_EXPORT const int Private;
extern PHPCPP_EXPORT const int Const;

/**
 *  Modifiers that are supported for methods and properties
 */
extern PHPCPP_EXPORT const int MethodModifiers;
extern PHPCPP_EXPORT const int PropertyModifiers;

/**
 *  End namespace
 */
}
