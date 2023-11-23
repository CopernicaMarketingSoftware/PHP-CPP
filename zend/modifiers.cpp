/**
 *  Modifiers.cpp
 *
 *  Modifiers implementation file
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  The modifiers are constants
 */
#if defined(_MSC_VER) && defined(BUILDING_PHPCPP)
   PHPCPP_EXPORT const int Static = 0;
   PHPCPP_EXPORT const int Abstract = 0;
   PHPCPP_EXPORT const int Final = 0;
   PHPCPP_EXPORT const int Public = 0;
   PHPCPP_EXPORT const int Protected = 0;
   PHPCPP_EXPORT const int Private = 0;
   PHPCPP_EXPORT const int Const = 0;

   /**
    *  Modifiers that are supported for methods and properties
    */
   PHPCPP_EXPORT const int MethodModifiers = 0;
   PHPCPP_EXPORT const int PropertyModifiers = 0;
#endif // defined(__VISUALC__)
/**
 *  End of namespace
 */
}
